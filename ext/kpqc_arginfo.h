/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0f7f0f1ee3826a55d13430875eba88d43fe7d35d */

#if defined(KPQC_TESTING)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_kpqc_native_test_seed, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, entropy, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(KPQC_TESTING)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_kpqc_native_test_signature_context, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_KpqC_aimer128f, 0, 0, KpqC\\SignatureAlgorithm, 0)
ZEND_END_ARG_INFO()

#define arginfo_KpqC_aimer128s arginfo_KpqC_aimer128f

#define arginfo_KpqC_aimer192f arginfo_KpqC_aimer128f

#define arginfo_KpqC_aimer192s arginfo_KpqC_aimer128f

#define arginfo_KpqC_aimer256f arginfo_KpqC_aimer128f

#define arginfo_KpqC_aimer256s arginfo_KpqC_aimer128f

#define arginfo_KpqC_haetae2 arginfo_KpqC_aimer128f

#define arginfo_KpqC_haetae3 arginfo_KpqC_aimer128f

#define arginfo_KpqC_haetae5 arginfo_KpqC_aimer128f

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_KpqC_ntruplus768, 0, 0, KpqC\\KeyEncapsulationAlgorithm, 0)
ZEND_END_ARG_INFO()

#define arginfo_KpqC_ntruplus864 arginfo_KpqC_ntruplus768

#define arginfo_KpqC_ntruplus1152 arginfo_KpqC_ntruplus768

#define arginfo_KpqC_smaugt128 arginfo_KpqC_ntruplus768

#define arginfo_KpqC_smaugt192 arginfo_KpqC_ntruplus768

#define arginfo_KpqC_smaugt256 arginfo_KpqC_ntruplus768

#define arginfo_KpqC_timer arginfo_KpqC_ntruplus768

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_KpqC_SignatureSizes___construct, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, publicKey, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, secretKey, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, signature, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_KpqC_KemSizes___construct, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, publicKey, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, secretKey, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, ciphertext, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, sharedSecret, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_KpqC_KeyPair___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, publicKey, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, secretKey, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KpqC_KeyPair___debugInfo, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KpqC_KeyPair___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_KpqC_EncapsulatedSecret___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, ciphertext, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, sharedSecret, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_KpqC_EncapsulatedSecret___debugInfo arginfo_class_KpqC_KeyPair___debugInfo

#define arginfo_class_KpqC_EncapsulatedSecret___toString arginfo_class_KpqC_KeyPair___toString

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_KpqC_SignatureAlgorithm___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_KpqC_SignatureAlgorithm_generateKeyPair, 0, 0, KpqC\\KeyPair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KpqC_SignatureAlgorithm_sign, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, secretKey, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, context, IS_STRING, 0, "\'\'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KpqC_SignatureAlgorithm_verify, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, signature, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, publicKey, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, context, IS_STRING, 0, "\'\'")
ZEND_END_ARG_INFO()

#define arginfo_class_KpqC_SignatureAlgorithm___toString arginfo_class_KpqC_KeyPair___toString

#define arginfo_class_KpqC_KeyEncapsulationAlgorithm___construct arginfo_class_KpqC_SignatureAlgorithm___construct

#define arginfo_class_KpqC_KeyEncapsulationAlgorithm_generateKeyPair arginfo_class_KpqC_SignatureAlgorithm_generateKeyPair

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_KpqC_KeyEncapsulationAlgorithm_encapsulate, 0, 1, KpqC\\EncapsulatedSecret, 0)
	ZEND_ARG_TYPE_INFO(0, publicKey, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_KpqC_KeyEncapsulationAlgorithm_decapsulate, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ciphertext, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, secretKey, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_KpqC_KeyEncapsulationAlgorithm___toString arginfo_class_KpqC_KeyPair___toString


#if defined(KPQC_TESTING)
ZEND_FUNCTION(kpqc_native_test_seed);
#endif
#if defined(KPQC_TESTING)
ZEND_FUNCTION(kpqc_native_test_signature_context);
#endif
ZEND_FUNCTION(aimer128f);
ZEND_FUNCTION(aimer128s);
ZEND_FUNCTION(aimer192f);
ZEND_FUNCTION(aimer192s);
ZEND_FUNCTION(aimer256f);
ZEND_FUNCTION(aimer256s);
ZEND_FUNCTION(haetae2);
ZEND_FUNCTION(haetae3);
ZEND_FUNCTION(haetae5);
ZEND_FUNCTION(ntruplus768);
ZEND_FUNCTION(ntruplus864);
ZEND_FUNCTION(ntruplus1152);
ZEND_FUNCTION(smaugt128);
ZEND_FUNCTION(smaugt192);
ZEND_FUNCTION(smaugt256);
ZEND_FUNCTION(timer);
ZEND_METHOD(KpqC_SignatureSizes, __construct);
ZEND_METHOD(KpqC_KemSizes, __construct);
ZEND_METHOD(KpqC_KeyPair, __construct);
ZEND_METHOD(KpqC_KeyPair, __debugInfo);
ZEND_METHOD(KpqC_KeyPair, __toString);
ZEND_METHOD(KpqC_EncapsulatedSecret, __construct);
ZEND_METHOD(KpqC_EncapsulatedSecret, __debugInfo);
ZEND_METHOD(KpqC_EncapsulatedSecret, __toString);
ZEND_METHOD(KpqC_SignatureAlgorithm, __construct);
ZEND_METHOD(KpqC_SignatureAlgorithm, generateKeyPair);
ZEND_METHOD(KpqC_SignatureAlgorithm, sign);
ZEND_METHOD(KpqC_SignatureAlgorithm, verify);
ZEND_METHOD(KpqC_SignatureAlgorithm, __toString);
ZEND_METHOD(KpqC_KeyEncapsulationAlgorithm, __construct);
ZEND_METHOD(KpqC_KeyEncapsulationAlgorithm, generateKeyPair);
ZEND_METHOD(KpqC_KeyEncapsulationAlgorithm, encapsulate);
ZEND_METHOD(KpqC_KeyEncapsulationAlgorithm, decapsulate);
ZEND_METHOD(KpqC_KeyEncapsulationAlgorithm, __toString);


static const zend_function_entry ext_functions[] = {
#if defined(KPQC_TESTING)
	ZEND_FE(kpqc_native_test_seed, arginfo_kpqc_native_test_seed)
#endif
#if defined(KPQC_TESTING)
	ZEND_FE(kpqc_native_test_signature_context, arginfo_kpqc_native_test_signature_context)
#endif
	ZEND_NS_FE("KpqC", aimer128f, arginfo_KpqC_aimer128f)
	ZEND_NS_FE("KpqC", aimer128s, arginfo_KpqC_aimer128s)
	ZEND_NS_FE("KpqC", aimer192f, arginfo_KpqC_aimer192f)
	ZEND_NS_FE("KpqC", aimer192s, arginfo_KpqC_aimer192s)
	ZEND_NS_FE("KpqC", aimer256f, arginfo_KpqC_aimer256f)
	ZEND_NS_FE("KpqC", aimer256s, arginfo_KpqC_aimer256s)
	ZEND_NS_FE("KpqC", haetae2, arginfo_KpqC_haetae2)
	ZEND_NS_FE("KpqC", haetae3, arginfo_KpqC_haetae3)
	ZEND_NS_FE("KpqC", haetae5, arginfo_KpqC_haetae5)
	ZEND_NS_FE("KpqC", ntruplus768, arginfo_KpqC_ntruplus768)
	ZEND_NS_FE("KpqC", ntruplus864, arginfo_KpqC_ntruplus864)
	ZEND_NS_FE("KpqC", ntruplus1152, arginfo_KpqC_ntruplus1152)
	ZEND_NS_FE("KpqC", smaugt128, arginfo_KpqC_smaugt128)
	ZEND_NS_FE("KpqC", smaugt192, arginfo_KpqC_smaugt192)
	ZEND_NS_FE("KpqC", smaugt256, arginfo_KpqC_smaugt256)
	ZEND_NS_FE("KpqC", timer, arginfo_KpqC_timer)
	ZEND_FE_END
};


static const zend_function_entry class_KpqC_SignatureSizes_methods[] = {
	ZEND_ME(KpqC_SignatureSizes, __construct, arginfo_class_KpqC_SignatureSizes___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_KpqC_KemSizes_methods[] = {
	ZEND_ME(KpqC_KemSizes, __construct, arginfo_class_KpqC_KemSizes___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_KpqC_KeyPair_methods[] = {
	ZEND_ME(KpqC_KeyPair, __construct, arginfo_class_KpqC_KeyPair___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_KeyPair, __debugInfo, arginfo_class_KpqC_KeyPair___debugInfo, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_KeyPair, __toString, arginfo_class_KpqC_KeyPair___toString, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_KpqC_EncapsulatedSecret_methods[] = {
	ZEND_ME(KpqC_EncapsulatedSecret, __construct, arginfo_class_KpqC_EncapsulatedSecret___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_EncapsulatedSecret, __debugInfo, arginfo_class_KpqC_EncapsulatedSecret___debugInfo, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_EncapsulatedSecret, __toString, arginfo_class_KpqC_EncapsulatedSecret___toString, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_KpqC_SignatureAlgorithm_methods[] = {
	ZEND_ME(KpqC_SignatureAlgorithm, __construct, arginfo_class_KpqC_SignatureAlgorithm___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(KpqC_SignatureAlgorithm, generateKeyPair, arginfo_class_KpqC_SignatureAlgorithm_generateKeyPair, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_SignatureAlgorithm, sign, arginfo_class_KpqC_SignatureAlgorithm_sign, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_SignatureAlgorithm, verify, arginfo_class_KpqC_SignatureAlgorithm_verify, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_SignatureAlgorithm, __toString, arginfo_class_KpqC_SignatureAlgorithm___toString, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_KpqC_KeyEncapsulationAlgorithm_methods[] = {
	ZEND_ME(KpqC_KeyEncapsulationAlgorithm, __construct, arginfo_class_KpqC_KeyEncapsulationAlgorithm___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(KpqC_KeyEncapsulationAlgorithm, generateKeyPair, arginfo_class_KpqC_KeyEncapsulationAlgorithm_generateKeyPair, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_KeyEncapsulationAlgorithm, encapsulate, arginfo_class_KpqC_KeyEncapsulationAlgorithm_encapsulate, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_KeyEncapsulationAlgorithm, decapsulate, arginfo_class_KpqC_KeyEncapsulationAlgorithm_decapsulate, ZEND_ACC_PUBLIC)
	ZEND_ME(KpqC_KeyEncapsulationAlgorithm, __toString, arginfo_class_KpqC_KeyEncapsulationAlgorithm___toString, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_KpqC_SignatureSizes(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "KpqC", "SignatureSizes", class_KpqC_SignatureSizes_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zval property_publicKey_default_value;
	ZVAL_UNDEF(&property_publicKey_default_value);
	zend_string *property_publicKey_name = zend_string_init("publicKey", sizeof("publicKey") - 1, 1);
	zend_declare_typed_property(class_entry, property_publicKey_name, &property_publicKey_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_publicKey_name);

	zval property_secretKey_default_value;
	ZVAL_UNDEF(&property_secretKey_default_value);
	zend_string *property_secretKey_name = zend_string_init("secretKey", sizeof("secretKey") - 1, 1);
	zend_declare_typed_property(class_entry, property_secretKey_name, &property_secretKey_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_secretKey_name);

	zval property_signature_default_value;
	ZVAL_UNDEF(&property_signature_default_value);
	zend_string *property_signature_name = zend_string_init("signature", sizeof("signature") - 1, 1);
	zend_declare_typed_property(class_entry, property_signature_name, &property_signature_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_signature_name);

	return class_entry;
}

static zend_class_entry *register_class_KpqC_KemSizes(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "KpqC", "KemSizes", class_KpqC_KemSizes_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zval property_publicKey_default_value;
	ZVAL_UNDEF(&property_publicKey_default_value);
	zend_string *property_publicKey_name = zend_string_init("publicKey", sizeof("publicKey") - 1, 1);
	zend_declare_typed_property(class_entry, property_publicKey_name, &property_publicKey_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_publicKey_name);

	zval property_secretKey_default_value;
	ZVAL_UNDEF(&property_secretKey_default_value);
	zend_string *property_secretKey_name = zend_string_init("secretKey", sizeof("secretKey") - 1, 1);
	zend_declare_typed_property(class_entry, property_secretKey_name, &property_secretKey_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_secretKey_name);

	zval property_ciphertext_default_value;
	ZVAL_UNDEF(&property_ciphertext_default_value);
	zend_string *property_ciphertext_name = zend_string_init("ciphertext", sizeof("ciphertext") - 1, 1);
	zend_declare_typed_property(class_entry, property_ciphertext_name, &property_ciphertext_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_ciphertext_name);

	zval property_sharedSecret_default_value;
	ZVAL_UNDEF(&property_sharedSecret_default_value);
	zend_string *property_sharedSecret_name = zend_string_init("sharedSecret", sizeof("sharedSecret") - 1, 1);
	zend_declare_typed_property(class_entry, property_sharedSecret_name, &property_sharedSecret_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_sharedSecret_name);

	return class_entry;
}

static zend_class_entry *register_class_KpqC_KeyPair(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "KpqC", "KeyPair", class_KpqC_KeyPair_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zval property_publicKey_default_value;
	ZVAL_UNDEF(&property_publicKey_default_value);
	zend_string *property_publicKey_name = zend_string_init("publicKey", sizeof("publicKey") - 1, 1);
	zend_declare_typed_property(class_entry, property_publicKey_name, &property_publicKey_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_publicKey_name);

	zval property_secretKey_default_value;
	ZVAL_UNDEF(&property_secretKey_default_value);
	zend_string *property_secretKey_name = zend_string_init("secretKey", sizeof("secretKey") - 1, 1);
	zend_declare_typed_property(class_entry, property_secretKey_name, &property_secretKey_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_secretKey_name);

	return class_entry;
}

static zend_class_entry *register_class_KpqC_EncapsulatedSecret(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "KpqC", "EncapsulatedSecret", class_KpqC_EncapsulatedSecret_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zval property_ciphertext_default_value;
	ZVAL_UNDEF(&property_ciphertext_default_value);
	zend_string *property_ciphertext_name = zend_string_init("ciphertext", sizeof("ciphertext") - 1, 1);
	zend_declare_typed_property(class_entry, property_ciphertext_name, &property_ciphertext_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_ciphertext_name);

	zval property_sharedSecret_default_value;
	ZVAL_UNDEF(&property_sharedSecret_default_value);
	zend_string *property_sharedSecret_name = zend_string_init("sharedSecret", sizeof("sharedSecret") - 1, 1);
	zend_declare_typed_property(class_entry, property_sharedSecret_name, &property_sharedSecret_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_sharedSecret_name);

	return class_entry;
}

static zend_class_entry *register_class_KpqC_SignatureAlgorithm(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "KpqC", "SignatureAlgorithm", class_KpqC_SignatureAlgorithm_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zval property_id_default_value;
	ZVAL_UNDEF(&property_id_default_value);
	zend_string *property_id_name = zend_string_init("id", sizeof("id") - 1, 1);
	zend_declare_typed_property(class_entry, property_id_name, &property_id_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_id_name);

	zend_string *property_sizes_class_KpqC_SignatureSizes = zend_string_init("KpqC\\SignatureSizes", sizeof("KpqC\\SignatureSizes")-1, 1);
	zval property_sizes_default_value;
	ZVAL_UNDEF(&property_sizes_default_value);
	zend_string *property_sizes_name = zend_string_init("sizes", sizeof("sizes") - 1, 1);
	zend_declare_typed_property(class_entry, property_sizes_name, &property_sizes_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_sizes_class_KpqC_SignatureSizes, 0, 0));
	zend_string_release(property_sizes_name);

	zval property_nativeName_default_value;
	ZVAL_UNDEF(&property_nativeName_default_value);
	zend_string *property_nativeName_name = zend_string_init("nativeName", sizeof("nativeName") - 1, 1);
	zend_declare_typed_property(class_entry, property_nativeName_name, &property_nativeName_default_value, ZEND_ACC_PRIVATE|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_nativeName_name);

	return class_entry;
}

static zend_class_entry *register_class_KpqC_KeyEncapsulationAlgorithm(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "KpqC", "KeyEncapsulationAlgorithm", class_KpqC_KeyEncapsulationAlgorithm_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zval property_id_default_value;
	ZVAL_UNDEF(&property_id_default_value);
	zend_string *property_id_name = zend_string_init("id", sizeof("id") - 1, 1);
	zend_declare_typed_property(class_entry, property_id_name, &property_id_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_id_name);

	zend_string *property_sizes_class_KpqC_KemSizes = zend_string_init("KpqC\\KemSizes", sizeof("KpqC\\KemSizes")-1, 1);
	zval property_sizes_default_value;
	ZVAL_UNDEF(&property_sizes_default_value);
	zend_string *property_sizes_name = zend_string_init("sizes", sizeof("sizes") - 1, 1);
	zend_declare_typed_property(class_entry, property_sizes_name, &property_sizes_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_sizes_class_KpqC_KemSizes, 0, 0));
	zend_string_release(property_sizes_name);

	zval property_nativeName_default_value;
	ZVAL_UNDEF(&property_nativeName_default_value);
	zend_string *property_nativeName_name = zend_string_init("nativeName", sizeof("nativeName") - 1, 1);
	zend_declare_typed_property(class_entry, property_nativeName_name, &property_nativeName_default_value, ZEND_ACC_PRIVATE|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release(property_nativeName_name);

	return class_entry;
}
