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

function requireKat(bool $condition, string $message): void
{
    if (!$condition) {
        throw new RuntimeException($message);
    }
}

function decodeHex(string $value): string
{
    $decoded = hex2bin($value);
    if ($decoded === false) {
        throw new RuntimeException('invalid hexadecimal KAT field');
    }
    return $decoded;
}

/**
 * @return Generator<int, array<string, string>>
 */
function readKatVectors(string $path): Generator
{
    $input = fopen($path, 'rb');
    if ($input === false) {
        throw new RuntimeException("missing KAT file: {$path}");
    }

    try {
        $vector = [];
        while (($line = fgets($input)) !== false) {
            $separator = strpos($line, ' = ');
            if ($separator === false) {
                continue;
            }
            $name = substr($line, 0, $separator);
            $value = rtrim(substr($line, $separator + 3), "\r\n");
            if ($name === 'count' && $vector !== []) {
                yield $vector;
                $vector = [];
            }
            $vector[$name] = $value;
        }
        if ($vector !== []) {
            yield $vector;
        }
    } finally {
        fclose($input);
    }
}

/** @param array<string, string> $vector */
function katField(array $vector, string $name): string
{
    if (!array_key_exists($name, $vector)) {
        throw new RuntimeException("missing KAT field: {$name}");
    }
    return $vector[$name];
}

$configured = getenv('KPQC_TEST_VECTORS');
$vectorRoot = $configured !== false && $configured !== ''
    ? $configured
    : dirname(__DIR__, 2) . '/kpqc-test-vectors';

/** @var list<array{SignatureAlgorithm, string, string, bool}> $signatures */
$signatures = [
    [aimer128f(), 'aimer128f', 'aimer/128f', false],
    [aimer128s(), 'aimer128s', 'aimer/128s', false],
    [aimer192f(), 'aimer192f', 'aimer/192f', false],
    [aimer192s(), 'aimer192s', 'aimer/192s', false],
    [aimer256f(), 'aimer256f', 'aimer/256f', false],
    [aimer256s(), 'aimer256s', 'aimer/256s', false],
    [haetae2(), 'haetae2', 'haetae/mode2', true],
    [haetae3(), 'haetae3', 'haetae/mode3', true],
    [haetae5(), 'haetae5', 'haetae/mode5', true],
];

/** @var list<array{KeyEncapsulationAlgorithm, string, string}> $kems */
$kems = [
    [ntruplus768(), 'ntruplus768', 'ntruplus/768'],
    [ntruplus864(), 'ntruplus864', 'ntruplus/864'],
    [ntruplus1152(), 'ntruplus1152', 'ntruplus/1152'],
    [smaugt128(), 'smaugt128', 'smaugt/mode1'],
    [smaugt192(), 'smaugt192', 'smaugt/mode3'],
    [smaugt256(), 'smaugt256', 'smaugt/mode5'],
    [timer(), 'timer', 'smaugt/modet'],
];

$total = 0;
foreach ($signatures as [$algorithm, $nativeName, $relativePath, $usesContext]) {
    $count = 0;
    $path = "{$vectorRoot}/{$relativePath}/kat.rsp";
    foreach (readKatVectors($path) as $vector) {
        $label = "{$algorithm->id} count=" . katField($vector, 'count');
        kpqc_native_test_seed($nativeName, decodeHex(katField($vector, 'seed')));

        $keys = $algorithm->generateKeyPair();
        requireKat(
            hash_equals(decodeHex(katField($vector, 'pk')), $keys->publicKey),
            "{$label}: public key",
        );
        requireKat(
            hash_equals(decodeHex(katField($vector, 'sk')), $keys->secretKey),
            "{$label}: secret key",
        );

        $context = $usesContext
            ? kpqc_native_test_signature_context()
            : '';
        $message = decodeHex(katField($vector, 'msg'));
        $signature = $algorithm->sign($message, $keys->secretKey, $context);
        $expected = array_key_exists('sig', $vector)
            ? decodeHex($vector['sig'])
            : substr(decodeHex(katField($vector, 'sm')), strlen($message));
        requireKat(
            hash_equals($expected, $signature),
            "{$label}: signature",
        );
        requireKat(
            $algorithm->verify(
                $message,
                $signature,
                $keys->publicKey,
                $context,
            ),
            "{$label}: verification",
        );
        ++$count;
    }
    requireKat($count === 100, "{$algorithm->id}: vector count");
    $total += $count;
    echo "{$algorithm->id}: {$count} PHP KAT records passed\n";
}

foreach ($kems as [$algorithm, $nativeName, $relativePath]) {
    $count = 0;
    $path = "{$vectorRoot}/{$relativePath}/kat.rsp";
    foreach (readKatVectors($path) as $vector) {
        $label = "{$algorithm->id} count=" . katField($vector, 'count');
        kpqc_native_test_seed($nativeName, decodeHex(katField($vector, 'seed')));

        $keys = $algorithm->generateKeyPair();
        requireKat(
            hash_equals(decodeHex(katField($vector, 'pk')), $keys->publicKey),
            "{$label}: public key",
        );
        requireKat(
            hash_equals(decodeHex(katField($vector, 'sk')), $keys->secretKey),
            "{$label}: secret key",
        );
        $result = $algorithm->encapsulate($keys->publicKey);
        requireKat(
            hash_equals(decodeHex(katField($vector, 'ct')), $result->ciphertext),
            "{$label}: ciphertext",
        );
        requireKat(
            hash_equals(
                decodeHex(katField($vector, 'ss')),
                $result->sharedSecret,
            ),
            "{$label}: shared secret",
        );
        requireKat(
            hash_equals(
                $result->sharedSecret,
                $algorithm->decapsulate(
                    $result->ciphertext,
                    $keys->secretKey,
                ),
            ),
            "{$label}: decapsulation",
        );
        ++$count;
    }
    requireKat($count === 100, "{$algorithm->id}: vector count");
    $total += $count;
    echo "{$algorithm->id}: {$count} PHP KAT records passed\n";
}

requireKat($total === 1600, 'total vector count');
echo "All 1,600 KAT records passed through the PHP API.\n";
