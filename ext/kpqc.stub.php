<?php

/** @generate-class-entries */

namespace {

#ifdef KPQC_TESTING
function kpqc_native_test_seed(string $name, string $entropy): void {}

function kpqc_native_test_signature_context(): string {}
#endif

}

namespace KpqC {

final class SignatureSizes
{
    public readonly int $publicKey;
    public readonly int $secretKey;
    public readonly int $signature;

    public function __construct(
        int $publicKey,
        int $secretKey,
        int $signature,
    ) {}
}

final class KemSizes
{
    public readonly int $publicKey;
    public readonly int $secretKey;
    public readonly int $ciphertext;
    public readonly int $sharedSecret;

    public function __construct(
        int $publicKey,
        int $secretKey,
        int $ciphertext,
        int $sharedSecret,
    ) {}
}

final class KeyPair
{
    public readonly string $publicKey;
    public readonly string $secretKey;

    public function __construct(string $publicKey, string $secretKey) {}

    public function __debugInfo(): array {}

    public function __toString(): string {}
}

final class EncapsulatedSecret
{
    public readonly string $ciphertext;
    public readonly string $sharedSecret;

    public function __construct(string $ciphertext, string $sharedSecret) {}

    public function __debugInfo(): array {}

    public function __toString(): string {}
}

final class SignatureAlgorithm
{
    public readonly string $id;
    public readonly SignatureSizes $sizes;
    private readonly string $nativeName;

    private function __construct() {}

    public function generateKeyPair(): KeyPair {}

    public function sign(
        string $message,
        string $secretKey,
        string $context = '',
    ): string {}

    public function verify(
        string $message,
        string $signature,
        string $publicKey,
        string $context = '',
    ): bool {}

    public function __toString(): string {}
}

final class KeyEncapsulationAlgorithm
{
    public readonly string $id;
    public readonly KemSizes $sizes;
    private readonly string $nativeName;

    private function __construct() {}

    public function generateKeyPair(): KeyPair {}

    public function encapsulate(string $publicKey): EncapsulatedSecret {}

    public function decapsulate(string $ciphertext, string $secretKey): string {}

    public function __toString(): string {}
}

function aimer128f(): SignatureAlgorithm {}
function aimer128s(): SignatureAlgorithm {}
function aimer192f(): SignatureAlgorithm {}
function aimer192s(): SignatureAlgorithm {}
function aimer256f(): SignatureAlgorithm {}
function aimer256s(): SignatureAlgorithm {}
function haetae2(): SignatureAlgorithm {}
function haetae3(): SignatureAlgorithm {}
function haetae5(): SignatureAlgorithm {}
function ntruplus768(): KeyEncapsulationAlgorithm {}
function ntruplus864(): KeyEncapsulationAlgorithm {}
function ntruplus1152(): KeyEncapsulationAlgorithm {}
function smaugt128(): KeyEncapsulationAlgorithm {}
function smaugt192(): KeyEncapsulationAlgorithm {}
function smaugt256(): KeyEncapsulationAlgorithm {}
function timer(): KeyEncapsulationAlgorithm {}

}
