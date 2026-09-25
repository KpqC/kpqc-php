<?php

declare(strict_types=1);

use KpqC\KeyEncapsulationAlgorithm;
use KpqC\SignatureAlgorithm;
use function KpqC\aimer128f;
use function KpqC\aimer128s;
use function KpqC\aimer192f;
use function KpqC\aimer192s;
use function KpqC\aimer256f;
use function KpqC\aimer256s;
use function KpqC\haetae2;
use function KpqC\haetae3;
use function KpqC\haetae5;
use function KpqC\ntruplus768;
use function KpqC\ntruplus864;
use function KpqC\ntruplus1152;
use function KpqC\smaugt128;
use function KpqC\smaugt192;
use function KpqC\smaugt256;
use function KpqC\timer;

function check(bool $condition, string $message): void
{
    if (!$condition) {
        throw new RuntimeException($message);
    }
}

function changed(string $value): string
{
    return chr(ord($value[0]) ^ 0x80) . substr($value, 1);
}

/** @param class-string<Throwable> $expected */
function expectException(string $expected, callable $operation): void
{
    try {
        $operation();
    } catch (Throwable $error) {
        check(
            $error instanceof $expected,
            sprintf('expected %s, received %s', $expected, $error::class),
        );
        return;
    }
    throw new RuntimeException("expected {$expected}");
}

$signatures = [
    aimer128f(), aimer128s(), aimer192f(), aimer192s(),
    aimer256f(), aimer256s(), haetae2(), haetae3(), haetae5(),
];
$kems = [
    ntruplus768(), ntruplus864(), ntruplus1152(),
    smaugt128(), smaugt192(), smaugt256(), timer(),
];

foreach ($signatures as $algorithm) {
    check($algorithm instanceof SignatureAlgorithm, "{$algorithm->id}: type");
    $keys = $algorithm->generateKeyPair();
    $other = $algorithm->generateKeyPair();
    check(strlen($keys->publicKey) === $algorithm->sizes->publicKey, "{$algorithm->id}: public key size");
    check(strlen($keys->secretKey) === $algorithm->sizes->secretKey, "{$algorithm->id}: secret key size");
    check($keys->publicKey !== $other->publicKey, "{$algorithm->id}: fresh key pair");

    foreach ([["test\0\xff", 'domain'], ['', ''], ['boundary', str_repeat('x', 255)]] as [$message, $context]) {
        $signature = $algorithm->sign($message, $keys->secretKey, $context);
        check(strlen($signature) === $algorithm->sizes->signature, "{$algorithm->id}: signature size");
        check($algorithm->verify($message, $signature, $keys->publicKey, $context), "{$algorithm->id}: verification");
        check(!$algorithm->verify($message . '!', $signature, $keys->publicKey, $context), "{$algorithm->id}: changed message");
        check(!$algorithm->verify($message, changed($signature), $keys->publicKey, $context), "{$algorithm->id}: changed signature");
        check(!$algorithm->verify($message, $signature, $other->publicKey, $context), "{$algorithm->id}: wrong public key");
        check(!$algorithm->verify($message, $signature, $keys->publicKey, 'wrong'), "{$algorithm->id}: wrong context");
    }

    check(!$algorithm->verify('', '', $keys->publicKey), "{$algorithm->id}: wrong signature size");
    expectException(InvalidArgumentException::class, fn () => $algorithm->sign('', $keys->secretKey, str_repeat('x', 256)));
    expectException(InvalidArgumentException::class, fn () => $algorithm->sign('', substr($keys->secretKey, 1)));
    expectException(InvalidArgumentException::class, fn () => $algorithm->verify('', '', substr($keys->publicKey, 1)));
}

foreach ($kems as $algorithm) {
    check($algorithm instanceof KeyEncapsulationAlgorithm, "{$algorithm->id}: type");
    $keys = $algorithm->generateKeyPair();
    $first = $algorithm->encapsulate($keys->publicKey);
    $second = $algorithm->encapsulate($keys->publicKey);
    check(strlen($keys->publicKey) === $algorithm->sizes->publicKey, "{$algorithm->id}: public key size");
    check(strlen($keys->secretKey) === $algorithm->sizes->secretKey, "{$algorithm->id}: secret key size");
    check(strlen($first->ciphertext) === $algorithm->sizes->ciphertext, "{$algorithm->id}: ciphertext size");
    check(strlen($first->sharedSecret) === $algorithm->sizes->sharedSecret, "{$algorithm->id}: secret size");
    check($algorithm->decapsulate($first->ciphertext, $keys->secretKey) === $first->sharedSecret, "{$algorithm->id}: decapsulation");
    check($first->ciphertext !== $second->ciphertext, "{$algorithm->id}: fresh ciphertext");
    check($first->sharedSecret !== $second->sharedSecret, "{$algorithm->id}: fresh shared secret");

    if (str_starts_with($algorithm->id, 'NTRU+')) {
        expectException(Exception::class, fn () => $algorithm->decapsulate(changed($first->ciphertext), $keys->secretKey));
        expectException(Exception::class, fn () => $algorithm->encapsulate(str_repeat("\xff", $algorithm->sizes->publicKey)));
    } else {
        $rejected = $algorithm->decapsulate(changed($first->ciphertext), $keys->secretKey);
        check(strlen($rejected) === $algorithm->sizes->sharedSecret, "{$algorithm->id}: implicit rejection size");
        check($rejected !== $first->sharedSecret, "{$algorithm->id}: implicit rejection value");
    }

    expectException(InvalidArgumentException::class, fn () => $algorithm->encapsulate(substr($keys->publicKey, 1)));
    expectException(InvalidArgumentException::class, fn () => $algorithm->decapsulate(substr($first->ciphertext, 1), $keys->secretKey));
    expectException(InvalidArgumentException::class, fn () => $algorithm->decapsulate($first->ciphertext, substr($keys->secretKey, 1)));
}

$keyText = (string) ntruplus768()->generateKeyPair();
$secretText = (string) ntruplus768()->encapsulate(ntruplus768()->generateKeyPair()->publicKey);
check(str_contains($keyText, '[REDACTED]'), 'key string redacts the secret key');
check(str_contains($secretText, '[REDACTED]'), 'encapsulation string redacts the shared secret');

check(
    (new ReflectionClass(SignatureAlgorithm::class))->isInternal(),
    'signature API is native',
);
check(
    (new ReflectionFunction('KpqC\\aimer128f'))->isInternal(),
    'algorithm factories are native',
);
check(!function_exists('kpqc_native_metadata'), 'internal bridge API is absent');
check(!function_exists('kpqc_native_test_seed'), 'test entropy is absent from production');

check(count($signatures) === 9, 'signature algorithm count');
check(count($kems) === 7, 'KEM algorithm count');

echo "All PHP API checks passed across 16 parameter sets.\n";
