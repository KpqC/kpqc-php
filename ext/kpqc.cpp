/* SPDX-License-Identifier: MIT */

extern "C" {
#include <php.h>
#include <ext/spl/spl_exceptions.h>
#include <ext/standard/info.h>
#include <Zend/zend_exceptions.h>
}

#include "kpqc/kpqc.hpp"
#include "version.h"

#ifdef KPQC_TESTING
#include "kat_drbg.hpp"
#endif

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

zend_class_entry* signature_sizes_ce;
zend_class_entry* kem_sizes_ce;
zend_class_entry* key_pair_ce;
zend_class_entry* encapsulated_secret_ce;
zend_class_entry* signature_algorithm_ce;
zend_class_entry* key_encapsulation_algorithm_ce;

using SignatureAccessor = const kpqc::SignatureAlgorithm& (*)() noexcept;
using KemAccessor = const kpqc::KeyEncapsulationAlgorithm& (*)() noexcept;

struct SignatureEntry {
    std::string_view name;
    SignatureAccessor accessor;
};

struct KemEntry {
    std::string_view name;
    KemAccessor accessor;
};

class PhpInvalidArgument final : public std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
};

constexpr std::array<SignatureEntry, 9> signatures{{
    {"aimer128f", kpqc::aimer::aimer128f},
    {"aimer128s", kpqc::aimer::aimer128s},
    {"aimer192f", kpqc::aimer::aimer192f},
    {"aimer192s", kpqc::aimer::aimer192s},
    {"aimer256f", kpqc::aimer::aimer256f},
    {"aimer256s", kpqc::aimer::aimer256s},
    {"haetae2", kpqc::haetae::haetae2},
    {"haetae3", kpqc::haetae::haetae3},
    {"haetae5", kpqc::haetae::haetae5},
}};

constexpr std::array<KemEntry, 7> kems{{
    {"ntruplus768", kpqc::ntruplus::ntruplus768},
    {"ntruplus864", kpqc::ntruplus::ntruplus864},
    {"ntruplus1152", kpqc::ntruplus::ntruplus1152},
    {"smaugt128", kpqc::smaugt::smaugt128},
    {"smaugt192", kpqc::smaugt::smaugt192},
    {"smaugt256", kpqc::smaugt::smaugt256},
    {"timer", kpqc::smaugt::timer},
}};

#ifdef KPQC_TESTING
using EntropyCallback = int (*)(std::uint8_t*, std::size_t);
using EntropySetter = void (*)(EntropyCallback);

extern "C" {
#define KPQC_DECLARE_SETTER(name) \
    void kpqc_##name##_set_randombytes(EntropyCallback)
KPQC_DECLARE_SETTER(aimer128f);
KPQC_DECLARE_SETTER(aimer128s);
KPQC_DECLARE_SETTER(aimer192f);
KPQC_DECLARE_SETTER(aimer192s);
KPQC_DECLARE_SETTER(aimer256f);
KPQC_DECLARE_SETTER(aimer256s);
KPQC_DECLARE_SETTER(haetae2);
KPQC_DECLARE_SETTER(haetae3);
KPQC_DECLARE_SETTER(haetae5);
KPQC_DECLARE_SETTER(ntruplus768);
KPQC_DECLARE_SETTER(ntruplus864);
KPQC_DECLARE_SETTER(ntruplus1152);
KPQC_DECLARE_SETTER(smaugt128);
KPQC_DECLARE_SETTER(smaugt192);
KPQC_DECLARE_SETTER(smaugt256);
KPQC_DECLARE_SETTER(timer);
#undef KPQC_DECLARE_SETTER
}

struct EntropyEntry {
    std::string_view name;
    EntropySetter setter;
};

constexpr std::array<EntropyEntry, 16> entropy_setters{{
    {"aimer128f", kpqc_aimer128f_set_randombytes},
    {"aimer128s", kpqc_aimer128s_set_randombytes},
    {"aimer192f", kpqc_aimer192f_set_randombytes},
    {"aimer192s", kpqc_aimer192s_set_randombytes},
    {"aimer256f", kpqc_aimer256f_set_randombytes},
    {"aimer256s", kpqc_aimer256s_set_randombytes},
    {"haetae2", kpqc_haetae2_set_randombytes},
    {"haetae3", kpqc_haetae3_set_randombytes},
    {"haetae5", kpqc_haetae5_set_randombytes},
    {"ntruplus768", kpqc_ntruplus768_set_randombytes},
    {"ntruplus864", kpqc_ntruplus864_set_randombytes},
    {"ntruplus1152", kpqc_ntruplus1152_set_randombytes},
    {"smaugt128", kpqc_smaugt128_set_randombytes},
    {"smaugt192", kpqc_smaugt192_set_randombytes},
    {"smaugt256", kpqc_smaugt256_set_randombytes},
    {"timer", kpqc_timer_set_randombytes},
}};

thread_local std::optional<kpqc::test::CtrDrbg> active_drbg;

int provide_test_entropy(std::uint8_t* output, std::size_t length) noexcept {
    if (!active_drbg.has_value() || (output == nullptr && length != 0)) {
        return -1;
    }
    try {
        const kpqc::Bytes generated = active_drbg->generate(length);
        std::copy(generated.begin(), generated.end(), output);
        return 0;
    } catch (...) {
        return -1;
    }
}

void select_test_entropy(std::string_view name) {
    for (const EntropyEntry& entry : entropy_setters) {
        if (entry.name == name) {
            entry.setter(provide_test_entropy);
            return;
        }
    }
    throw std::invalid_argument(
        "unknown algorithm: " + std::string(name));
}
#endif

std::string_view view(const zend_string* value) noexcept {
    return {ZSTR_VAL(value), ZSTR_LEN(value)};
}

kpqc::Bytes bytes(const zend_string* value) {
    const auto* first = reinterpret_cast<const std::uint8_t*>(ZSTR_VAL(value));
    return {first, first + ZSTR_LEN(value)};
}

const kpqc::SignatureAlgorithm* find_signature(std::string_view name) noexcept {
    for (const SignatureEntry& entry : signatures) {
        if (entry.name == name) {
            return &entry.accessor();
        }
    }
    return nullptr;
}

const kpqc::KeyEncapsulationAlgorithm* find_kem(std::string_view name) noexcept {
    for (const KemEntry& entry : kems) {
        if (entry.name == name) {
            return &entry.accessor();
        }
    }
    return nullptr;
}

const kpqc::SignatureAlgorithm& require_signature(const zend_string* name) {
    if (const auto* algorithm = find_signature(view(name)); algorithm != nullptr) {
        return *algorithm;
    }
    throw std::invalid_argument(
        "unknown signature algorithm: " + std::string(view(name)));
}

const kpqc::KeyEncapsulationAlgorithm& require_kem(const zend_string* name) {
    if (const auto* algorithm = find_kem(view(name)); algorithm != nullptr) {
        return *algorithm;
    }
    throw std::invalid_argument(
        "unknown key-encapsulation algorithm: " + std::string(view(name)));
}

void return_bytes(zval* return_value, const kpqc::Bytes& value) {
    const char* data = value.empty()
        ? ""
        : reinterpret_cast<const char*>(value.data());
    ZVAL_STRINGL(return_value, data, value.size());
}

void translate_exception() noexcept {
    try {
        throw;
    } catch (const std::bad_alloc&) {
        zend_throw_error(nullptr, "KpqC allocation failed");
    } catch (const PhpInvalidArgument& error) {
        zend_throw_exception(
            spl_ce_InvalidArgumentException, error.what(), 0);
    } catch (const std::invalid_argument& error) {
        zend_value_error("%s", error.what());
    } catch (const kpqc::Error& error) {
        zend_throw_exception(zend_ce_exception, error.what(), error.status());
    } catch (const std::exception& error) {
        zend_throw_exception(zend_ce_exception, error.what(), 0);
    } catch (...) {
        zend_throw_error(nullptr, "KpqC failed with an unknown native error");
    }
}

void require_size(
    const char* label,
    const zend_string* value,
    std::size_t expected) {
    const std::size_t actual = ZSTR_LEN(value);
    if (actual != expected) {
        throw PhpInvalidArgument(
            std::string(label) + " must be " + std::to_string(expected) +
            " bytes, received " + std::to_string(actual));
    }
}

void require_context(const zend_string* context) {
    if (ZSTR_LEN(context) > 255) {
        throw PhpInvalidArgument(
            "context cannot exceed 255 bytes, received " +
            std::to_string(ZSTR_LEN(context)));
    }
}

zend_string* read_string_property(
    zend_class_entry* scope,
    zend_object* object,
    const char* name,
    std::size_t length) {
    zval value;
    zval* property = zend_read_property(
        scope, object, name, length, false, &value);
    return Z_STR_P(property);
}

void initialize_string_property(
    zend_class_entry* scope,
    zend_object* object,
    const char* name,
    std::size_t name_length,
    const char* value,
    std::size_t value_length) {
    zend_update_property_stringl(
        scope, object, name, name_length, value, value_length);
}

void initialize_long_property(
    zend_class_entry* scope,
    zend_object* object,
    const char* name,
    std::size_t name_length,
    zend_long value) {
    zend_update_property_long(scope, object, name, name_length, value);
}

void initialize_object_property(
    zend_class_entry* scope,
    zend_object* object,
    const char* name,
    std::size_t name_length,
    zval* value) {
    zend_update_property(scope, object, name, name_length, value);
}

void create_signature_sizes(
    zval* value,
    const kpqc::SignatureSizes& sizes) {
    object_init_ex(value, signature_sizes_ce);
    initialize_long_property(
        signature_sizes_ce, Z_OBJ_P(value), ZEND_STRL("publicKey"),
        sizes.public_key);
    initialize_long_property(
        signature_sizes_ce, Z_OBJ_P(value), ZEND_STRL("secretKey"),
        sizes.secret_key);
    initialize_long_property(
        signature_sizes_ce, Z_OBJ_P(value), ZEND_STRL("signature"),
        sizes.signature);
}

void create_kem_sizes(zval* value, const kpqc::KemSizes& sizes) {
    object_init_ex(value, kem_sizes_ce);
    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(value), ZEND_STRL("publicKey"),
        sizes.public_key);
    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(value), ZEND_STRL("secretKey"),
        sizes.secret_key);
    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(value), ZEND_STRL("ciphertext"),
        sizes.ciphertext);
    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(value), ZEND_STRL("sharedSecret"),
        sizes.shared_secret);
}

void create_key_pair(zval* value, const kpqc::KeyPair& result) {
    object_init_ex(value, key_pair_ce);
    const char* public_key = result.public_key.empty()
        ? ""
        : reinterpret_cast<const char*>(result.public_key.data());
    const char* secret_key = result.secret_key.empty()
        ? ""
        : reinterpret_cast<const char*>(result.secret_key.data());
    initialize_string_property(
        key_pair_ce, Z_OBJ_P(value), ZEND_STRL("publicKey"),
        public_key, result.public_key.size());
    initialize_string_property(
        key_pair_ce, Z_OBJ_P(value), ZEND_STRL("secretKey"),
        secret_key, result.secret_key.size());
}

void create_encapsulated_secret(
    zval* value,
    const kpqc::EncapsulatedSecret& result) {
    object_init_ex(value, encapsulated_secret_ce);
    const char* ciphertext = result.ciphertext.empty()
        ? ""
        : reinterpret_cast<const char*>(result.ciphertext.data());
    const char* shared_secret = result.shared_secret.empty()
        ? ""
        : reinterpret_cast<const char*>(result.shared_secret.data());
    initialize_string_property(
        encapsulated_secret_ce, Z_OBJ_P(value), ZEND_STRL("ciphertext"),
        ciphertext, result.ciphertext.size());
    initialize_string_property(
        encapsulated_secret_ce, Z_OBJ_P(value), ZEND_STRL("sharedSecret"),
        shared_secret, result.shared_secret.size());
}

void create_signature_algorithm(zval* value, std::string_view name) {
    const auto* algorithm = find_signature(name);
    if (algorithm == nullptr) {
        throw std::invalid_argument(
            "unknown signature algorithm: " + std::string(name));
    }
    object_init_ex(value, signature_algorithm_ce);
    initialize_string_property(
        signature_algorithm_ce, Z_OBJ_P(value), ZEND_STRL("id"),
        algorithm->id().data(), algorithm->id().size());
    zval sizes;
    create_signature_sizes(&sizes, algorithm->sizes());
    initialize_object_property(
        signature_algorithm_ce, Z_OBJ_P(value), ZEND_STRL("sizes"), &sizes);
    zval_ptr_dtor(&sizes);
    initialize_string_property(
        signature_algorithm_ce, Z_OBJ_P(value), ZEND_STRL("nativeName"),
        name.data(), name.size());
}

void create_kem_algorithm(zval* value, std::string_view name) {
    const auto* algorithm = find_kem(name);
    if (algorithm == nullptr) {
        throw std::invalid_argument(
            "unknown key-encapsulation algorithm: " + std::string(name));
    }
    object_init_ex(value, key_encapsulation_algorithm_ce);
    initialize_string_property(
        key_encapsulation_algorithm_ce, Z_OBJ_P(value), ZEND_STRL("id"),
        algorithm->id().data(), algorithm->id().size());
    zval sizes;
    create_kem_sizes(&sizes, algorithm->sizes());
    initialize_object_property(
        key_encapsulation_algorithm_ce, Z_OBJ_P(value), ZEND_STRL("sizes"),
        &sizes);
    zval_ptr_dtor(&sizes);
    initialize_string_property(
        key_encapsulation_algorithm_ce, Z_OBJ_P(value),
        ZEND_STRL("nativeName"), name.data(), name.size());
}

}  // namespace

#ifdef KPQC_TESTING
PHP_FUNCTION(kpqc_native_test_seed) {
    static_cast<void>(return_value);
    zend_string* name;
    zend_string* seed;
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(name)
        Z_PARAM_STR(seed)
    ZEND_PARSE_PARAMETERS_END();

    try {
        select_test_entropy(view(name));
        active_drbg.emplace(bytes(seed));
    } catch (...) {
        translate_exception();
    }
}

PHP_FUNCTION(kpqc_native_test_signature_context) {
    ZEND_PARSE_PARAMETERS_NONE();

    try {
        if (!active_drbg.has_value()) {
            throw std::invalid_argument("test entropy is not initialized");
        }
        kpqc::test::CtrDrbg preview = *active_drbg;
        static_cast<void>(preview.generate(32));
        const kpqc::Bytes encoded_length = preview.generate(1);
        return_bytes(return_value, preview.generate(encoded_length.front()));
    } catch (...) {
        translate_exception();
    }
}
#endif

PHP_METHOD(KpqC_SignatureSizes, __construct) {
    (void)return_value;
    zend_long public_key;
    zend_long secret_key;
    zend_long signature;
    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_LONG(public_key)
        Z_PARAM_LONG(secret_key)
        Z_PARAM_LONG(signature)
    ZEND_PARSE_PARAMETERS_END();

    initialize_long_property(
        signature_sizes_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("publicKey"),
        public_key);
    initialize_long_property(
        signature_sizes_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("secretKey"),
        secret_key);
    initialize_long_property(
        signature_sizes_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("signature"),
        signature);
}

PHP_METHOD(KpqC_KemSizes, __construct) {
    (void)return_value;
    zend_long public_key;
    zend_long secret_key;
    zend_long ciphertext;
    zend_long shared_secret;
    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_LONG(public_key)
        Z_PARAM_LONG(secret_key)
        Z_PARAM_LONG(ciphertext)
        Z_PARAM_LONG(shared_secret)
    ZEND_PARSE_PARAMETERS_END();

    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("publicKey"), public_key);
    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("secretKey"), secret_key);
    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("ciphertext"), ciphertext);
    initialize_long_property(
        kem_sizes_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("sharedSecret"),
        shared_secret);
}

PHP_METHOD(KpqC_KeyPair, __construct) {
    (void)return_value;
    zend_string* public_key;
    zend_string* secret_key;
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(public_key)
        Z_PARAM_STR(secret_key)
    ZEND_PARSE_PARAMETERS_END();

    initialize_string_property(
        key_pair_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("publicKey"),
        ZSTR_VAL(public_key), ZSTR_LEN(public_key));
    initialize_string_property(
        key_pair_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("secretKey"),
        ZSTR_VAL(secret_key), ZSTR_LEN(secret_key));
}

PHP_METHOD(KpqC_KeyPair, __debugInfo) {
    ZEND_PARSE_PARAMETERS_NONE();
    zend_string* public_key = read_string_property(
        key_pair_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("publicKey"));
    array_init(return_value);
    add_assoc_str(
        return_value, "publicKey",
        strpprintf(0, "%zu bytes", ZSTR_LEN(public_key)));
    add_assoc_string(return_value, "secretKey", "[REDACTED]");
}

PHP_METHOD(KpqC_KeyPair, __toString) {
    ZEND_PARSE_PARAMETERS_NONE();
    zend_string* public_key = read_string_property(
        key_pair_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("publicKey"));
    RETURN_STR(strpprintf(
        0, "KeyPair(publicKey=%zu bytes, secretKey=[REDACTED])",
        ZSTR_LEN(public_key)));
}

PHP_METHOD(KpqC_EncapsulatedSecret, __construct) {
    (void)return_value;
    zend_string* ciphertext;
    zend_string* shared_secret;
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(ciphertext)
        Z_PARAM_STR(shared_secret)
    ZEND_PARSE_PARAMETERS_END();

    initialize_string_property(
        encapsulated_secret_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("ciphertext"),
        ZSTR_VAL(ciphertext), ZSTR_LEN(ciphertext));
    initialize_string_property(
        encapsulated_secret_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("sharedSecret"),
        ZSTR_VAL(shared_secret), ZSTR_LEN(shared_secret));
}

PHP_METHOD(KpqC_EncapsulatedSecret, __debugInfo) {
    ZEND_PARSE_PARAMETERS_NONE();
    zend_string* ciphertext = read_string_property(
        encapsulated_secret_ce, Z_OBJ_P(ZEND_THIS),
        ZEND_STRL("ciphertext"));
    array_init(return_value);
    add_assoc_str(
        return_value, "ciphertext",
        strpprintf(0, "%zu bytes", ZSTR_LEN(ciphertext)));
    add_assoc_string(return_value, "sharedSecret", "[REDACTED]");
}

PHP_METHOD(KpqC_EncapsulatedSecret, __toString) {
    ZEND_PARSE_PARAMETERS_NONE();
    zend_string* ciphertext = read_string_property(
        encapsulated_secret_ce, Z_OBJ_P(ZEND_THIS),
        ZEND_STRL("ciphertext"));
    RETURN_STR(strpprintf(
        0,
        "EncapsulatedSecret(ciphertext=%zu bytes, "
        "sharedSecret=[REDACTED])",
        ZSTR_LEN(ciphertext)));
}

PHP_METHOD(KpqC_SignatureAlgorithm, __construct) {
    (void)return_value;
    ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(KpqC_SignatureAlgorithm, generateKeyPair) {
    ZEND_PARSE_PARAMETERS_NONE();
    try {
        zend_string* name = read_string_property(
            signature_algorithm_ce, Z_OBJ_P(ZEND_THIS),
            ZEND_STRL("nativeName"));
        create_key_pair(return_value, require_signature(name).generate_key_pair());
    } catch (...) {
        translate_exception();
    }
}

PHP_METHOD(KpqC_SignatureAlgorithm, sign) {
    zend_string* message;
    zend_string* secret_key;
    zend_string* context = nullptr;
    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_STR(message)
        Z_PARAM_STR(secret_key)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR(context)
    ZEND_PARSE_PARAMETERS_END();

    try {
        zend_string* name = read_string_property(
            signature_algorithm_ce, Z_OBJ_P(ZEND_THIS),
            ZEND_STRL("nativeName"));
        const kpqc::SignatureAlgorithm& algorithm = require_signature(name);
        require_size("secretKey", secret_key, algorithm.sizes().secret_key);
        if (context != nullptr) {
            require_context(context);
        }
        const kpqc::Bytes result = algorithm.sign(
            bytes(message), bytes(secret_key),
            context == nullptr ? kpqc::Bytes{} : bytes(context));
        return_bytes(return_value, result);
    } catch (...) {
        translate_exception();
    }
}

PHP_METHOD(KpqC_SignatureAlgorithm, verify) {
    zend_string* message;
    zend_string* signature;
    zend_string* public_key;
    zend_string* context = nullptr;
    ZEND_PARSE_PARAMETERS_START(3, 4)
        Z_PARAM_STR(message)
        Z_PARAM_STR(signature)
        Z_PARAM_STR(public_key)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR(context)
    ZEND_PARSE_PARAMETERS_END();

    try {
        zend_string* name = read_string_property(
            signature_algorithm_ce, Z_OBJ_P(ZEND_THIS),
            ZEND_STRL("nativeName"));
        const kpqc::SignatureAlgorithm& algorithm = require_signature(name);
        require_size("publicKey", public_key, algorithm.sizes().public_key);
        if (context != nullptr) {
            require_context(context);
        }
        if (ZSTR_LEN(signature) != algorithm.sizes().signature) {
            RETURN_FALSE;
        }
        RETURN_BOOL(algorithm.verify(
            bytes(message), bytes(signature), bytes(public_key),
            context == nullptr ? kpqc::Bytes{} : bytes(context)));
    } catch (...) {
        translate_exception();
    }
}

PHP_METHOD(KpqC_SignatureAlgorithm, __toString) {
    ZEND_PARSE_PARAMETERS_NONE();
    zend_string* id = read_string_property(
        signature_algorithm_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("id"));
    RETURN_STR_COPY(id);
}

PHP_METHOD(KpqC_KeyEncapsulationAlgorithm, __construct) {
    (void)return_value;
    ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(KpqC_KeyEncapsulationAlgorithm, generateKeyPair) {
    ZEND_PARSE_PARAMETERS_NONE();
    try {
        zend_string* name = read_string_property(
            key_encapsulation_algorithm_ce, Z_OBJ_P(ZEND_THIS),
            ZEND_STRL("nativeName"));
        create_key_pair(return_value, require_kem(name).generate_key_pair());
    } catch (...) {
        translate_exception();
    }
}

PHP_METHOD(KpqC_KeyEncapsulationAlgorithm, encapsulate) {
    zend_string* public_key;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(public_key)
    ZEND_PARSE_PARAMETERS_END();

    try {
        zend_string* name = read_string_property(
            key_encapsulation_algorithm_ce, Z_OBJ_P(ZEND_THIS),
            ZEND_STRL("nativeName"));
        const kpqc::KeyEncapsulationAlgorithm& algorithm = require_kem(name);
        require_size("publicKey", public_key, algorithm.sizes().public_key);
        create_encapsulated_secret(
            return_value, algorithm.encapsulate(bytes(public_key)));
    } catch (...) {
        translate_exception();
    }
}

PHP_METHOD(KpqC_KeyEncapsulationAlgorithm, decapsulate) {
    zend_string* ciphertext;
    zend_string* secret_key;
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(ciphertext)
        Z_PARAM_STR(secret_key)
    ZEND_PARSE_PARAMETERS_END();

    try {
        zend_string* name = read_string_property(
            key_encapsulation_algorithm_ce, Z_OBJ_P(ZEND_THIS),
            ZEND_STRL("nativeName"));
        const kpqc::KeyEncapsulationAlgorithm& algorithm = require_kem(name);
        require_size("ciphertext", ciphertext, algorithm.sizes().ciphertext);
        require_size("secretKey", secret_key, algorithm.sizes().secret_key);
        return_bytes(
            return_value,
            algorithm.decapsulate(bytes(ciphertext), bytes(secret_key)));
    } catch (...) {
        translate_exception();
    }
}

PHP_METHOD(KpqC_KeyEncapsulationAlgorithm, __toString) {
    ZEND_PARSE_PARAMETERS_NONE();
    zend_string* id = read_string_property(
        key_encapsulation_algorithm_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("id"));
    RETURN_STR_COPY(id);
}

#define KPQC_SIGNATURE_FACTORY(function_name) \
    PHP_FUNCTION(function_name) {             \
        ZEND_PARSE_PARAMETERS_NONE();         \
        try {                                 \
            create_signature_algorithm(       \
                return_value, #function_name);\
        } catch (...) {                       \
            translate_exception();            \
        }                                     \
    }

KPQC_SIGNATURE_FACTORY(aimer128f)
KPQC_SIGNATURE_FACTORY(aimer128s)
KPQC_SIGNATURE_FACTORY(aimer192f)
KPQC_SIGNATURE_FACTORY(aimer192s)
KPQC_SIGNATURE_FACTORY(aimer256f)
KPQC_SIGNATURE_FACTORY(aimer256s)
KPQC_SIGNATURE_FACTORY(haetae2)
KPQC_SIGNATURE_FACTORY(haetae3)
KPQC_SIGNATURE_FACTORY(haetae5)
#undef KPQC_SIGNATURE_FACTORY

#define KPQC_KEM_FACTORY(function_name) \
    PHP_FUNCTION(function_name) {       \
        ZEND_PARSE_PARAMETERS_NONE();   \
        try {                           \
            create_kem_algorithm(       \
                return_value, #function_name);\
        } catch (...) {                 \
            translate_exception();      \
        }                               \
    }

KPQC_KEM_FACTORY(ntruplus768)
KPQC_KEM_FACTORY(ntruplus864)
KPQC_KEM_FACTORY(ntruplus1152)
KPQC_KEM_FACTORY(smaugt128)
KPQC_KEM_FACTORY(smaugt192)
KPQC_KEM_FACTORY(smaugt256)
KPQC_KEM_FACTORY(timer)
#undef KPQC_KEM_FACTORY

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#endif
#include "kpqc_arginfo.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

PHP_MINIT_FUNCTION(kpqc) {
    (void)type;
    (void)module_number;
    REGISTER_STRING_CONSTANT(
        "KPQC_VERSION", PHP_KPQC_VERSION, CONST_CS | CONST_PERSISTENT);
    signature_sizes_ce = register_class_KpqC_SignatureSizes();
    kem_sizes_ce = register_class_KpqC_KemSizes();
    key_pair_ce = register_class_KpqC_KeyPair();
    encapsulated_secret_ce = register_class_KpqC_EncapsulatedSecret();
    signature_algorithm_ce = register_class_KpqC_SignatureAlgorithm();
    key_encapsulation_algorithm_ce =
        register_class_KpqC_KeyEncapsulationAlgorithm();
    return SUCCESS;
}

PHP_MINFO_FUNCTION(kpqc) {
    (void)zend_module;
    php_info_print_table_start();
    php_info_print_table_row(2, "KpqC support", "enabled");
    php_info_print_table_row(2, "KpqC version", PHP_KPQC_VERSION);
    php_info_print_table_end();
}

zend_module_entry kpqc_module_entry = {
    STANDARD_MODULE_HEADER,
    "kpqc",
    ext_functions,
    PHP_MINIT(kpqc),
    nullptr,
    nullptr,
    nullptr,
    PHP_MINFO(kpqc),
    PHP_KPQC_VERSION,
    STANDARD_MODULE_PROPERTIES,
};

extern "C" {
ZEND_GET_MODULE(kpqc)
}
