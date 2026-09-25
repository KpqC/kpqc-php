# KpqC

KpqC provides typed, synchronous PHP APIs for AIMer, HAETAE, NTRU+,
and SMAUG-T.

## Requirements

- PHP 8.1 or newer
- macOS or Linux
- PIE and the standard PHP extension build tools (`phpize`, `php-config`,
  Autoconf, and Make)
- CMake 3.20 or newer and C11/C++17 compilers during installation

## Install

Install the extension from Packagist with
[PIE](https://github.com/php/pie):

```sh
pie install kpqc/kpqc
```

PIE builds, installs, and enables the extension for the selected PHP
installation. Confirm that it is loaded with:

```sh
php --ri kpqc
```

### Build from a source checkout

For development, the repository can also be built directly with CMake:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
php -d extension="$(pwd)/build/kpqc.so" tests/api_test.php
```

To install the compiled module into the extension directory reported by the
selected `php-config`, run:

```sh
cmake --install build
```

The extension directory may require elevated write permission. After
installation, enable the module through `php.ini`:

```ini
extension=/absolute/path/to/kpqc.so
```

## Available schemes

| Algorithm | Type | Functions |
| --- | --- | --- |
| **AIMer** | Signature | `aimer128f`, `aimer128s`, `aimer192f`, `aimer192s`, `aimer256f`, `aimer256s` |
| **HAETAE** | Signature | `haetae2`, `haetae3`, `haetae5` |
| **NTRU+** | Key encapsulation | `ntruplus768`, `ntruplus864`, `ntruplus1152` |
| **SMAUG&#8209;T** | Key encapsulation | `smaugt128`, `smaugt192`, `smaugt256`, `timer` |

Named functions return immutable algorithm objects:

```php
<?php

use function KpqC\aimer128f;

$algorithm = aimer128f();
$payload = "release-manifest:v3";
$keys = $algorithm->generateKeyPair();
$proof = $algorithm->sign($payload, $keys->secretKey);

if (!$algorithm->verify($payload, $proof, $keys->publicKey)) {
    throw new RuntimeException('Signature verification failed');
}
```

### Signature contexts

AIMer and HAETAE accept an optional context. A context separates signatures
created for different application purposes and may contain up to 255 bytes:

```php
use function KpqC\haetae3;

$algorithm = haetae3();
$keys = $algorithm->generateKeyPair();
$signature = $algorithm->sign(
    'account=42',
    $keys->secretKey,
    'audit-record',
);

$valid = $algorithm->verify(
    'account=42',
    $signature,
    $keys->publicKey,
    'audit-record',
);
```

Verification fails when the supplied context does not match the one used for
signing.

### Key encapsulation

A KEM creates a shared secret for a sender and a recipient. The public key may
be distributed; the secret key and resulting shared secret must remain private.

```php
use function KpqC\smaugt192;

$algorithm = smaugt192();
$recipient = $algorithm->generateKeyPair();
$outbound = $algorithm->encapsulate($recipient->publicKey);

$inboundSecret = $algorithm->decapsulate(
    $outbound->ciphertext,
    $recipient->secretKey,
);

var_dump(hash_equals($outbound->sharedSecret, $inboundSecret)); // true
```

## Imports

Named algorithm functions can be imported individually:

```php
use function KpqC\aimer256s;
use function KpqC\haetae5;
use function KpqC\ntruplus1152;
use function KpqC\timer;
```

The shared algorithm classes and value objects are available from the `KpqC`
namespace:

```php
use KpqC\KeyEncapsulationAlgorithm;
use KpqC\SignatureAlgorithm;
use function KpqC\aimer192f;
use function KpqC\ntruplus864;

$signer = aimer192f();
$keyExchange = ntruplus864();

assert($signer instanceof SignatureAlgorithm);
assert($keyExchange instanceof KeyEncapsulationAlgorithm);
```

## Data and failures

PHP strings are treated as binary byte strings. Algorithm objects expose an
`id` and a read-only `sizes` object. Key pairs and encapsulation results are
read-only objects whose string and debug representations redact secrets.

### Parameter sizes

All sizes are in bytes.

#### Signatures

| Algorithm | Public key | Secret key | Signature |
| --- | ---: | ---: | ---: |
| `aimer128f` | 32 | 48 | 6,944 |
| `aimer128s` | 32 | 48 | 4,704 |
| `aimer192f` | 48 | 72 | 15,408 |
| `aimer192s` | 48 | 72 | 10,320 |
| `aimer256f` | 64 | 96 | 31,360 |
| `aimer256s` | 64 | 96 | 20,224 |
| `haetae2` | 992 | 1,408 | 1,474 |
| `haetae3` | 1,472 | 2,112 | 2,349 |
| `haetae5` | 2,080 | 2,752 | 2,948 |

#### Key encapsulation

| Algorithm | Public key | Secret key | Ciphertext | Shared secret |
| --- | ---: | ---: | ---: | ---: |
| `ntruplus768` | 1,152 | 2,336 | 1,152 | 32 |
| `ntruplus864` | 1,296 | 2,624 | 1,296 | 32 |
| `ntruplus1152` | 1,728 | 3,488 | 1,728 | 32 |
| `smaugt128` | 672 | 832 | 672 | 32 |
| `smaugt192` | 1,088 | 1,312 | 992 | 32 |
| `smaugt256` | 1,440 | 1,728 | 1,376 | 32 |
| `timer` | 672 | 832 | 608 | 32 |

Methods reject values of the wrong size. Signature verification returns
`false` for an invalid signature. NTRU+ rejects a non-canonical public key or
an invalid ciphertext. SMAUG-T performs implicit rejection and returns a
replacement secret instead; that value does not equal the sender's shared
secret.

## Known-answer tests

The bundled implementation is tested through the PHP API against all 1,600 KAT
records in
[KpqC/kpqc-test-vectors at commit 179dcc05ece2](https://github.com/KpqC/kpqc-test-vectors/tree/179dcc05ece2e22262cea1a61f3cdf1a5b08a304).
The vector files are not duplicated in this repository.

With `kpqc-test-vectors` checked out beside `kpqc-php`, run:

```sh
cmake -S . -B build-kat \
  -DCMAKE_BUILD_TYPE=Release \
  -DKPQC_BUILD_NATIVE_TESTS=ON
cmake --build build-kat --parallel
KPQC_TEST_VECTORS=../kpqc-test-vectors \
  ctest --test-dir build-kat --output-on-failure \
    -R '^(api|kat|php-api|php-kat)$'
```

## Distribution

`kpqc/kpqc` is a PIE extension package distributed through Packagist. The same
repository contains the PHP API and all native sources required for a source
build; installation does not download algorithm sources from another
repository. There are no runtime dependencies beyond PHP and the installed
`kpqc` extension.

## Security

The native cores are compiled from the upstream algorithm implementations.
This package has not received an independent security audit and does not
provide a constant-time execution guarantee. Assess those constraints before
using it with sensitive production keys.

Third-party licenses and attributions are listed in
[THIRD_PARTY_NOTICES.md](https://github.com/KpqC/kpqc-php/blob/main/THIRD_PARTY_NOTICES.md).
