#include "kuznechik.h"


unsigned char multiplicateInGaluaField(unsigned char a, unsigned char b)
{
    unsigned char c = 0x00;
    while (b > 0) {
        if (b & 1) {
            c ^= a;
        }
        a = (a << 1) ^ (a & 0x80 ? 0xC3 : 0x00);
        b >>= 1;
    }
    return c;
}

void performXTransformation(bytesVector &vec1, bytesVector &vec2, bytesVector &result)
{
    for (int i = 0; i < vec1.size(); ++i){
        result[i] = vec1[i] ^ vec2[i];
    }
}

void performSTransformation(bytesVector &vec)
{
    for (int i = 0; i < 16; ++i){
        vec[i] = pi[vec[i]];
    }
}

void performReverseSTransformation(bytesVector &vec)
{
    for (int i = 0; i < 16; ++i){
        vec[i] = reversePi[vec[i]];
    }
}

void performRTransformation(bytesVector &state)
{
    unsigned char a0 = state[15];
    for(int i = 14; i >= 0; --i){
        state[i+1] = state[i];
        a0 ^= multiplicateInGaluaField(state[i], factors[i]);
    }
    state[0] = a0;
}

void performReverseRTransformation(bytesVector &state)
{
    unsigned char a15 = state[0];
    for(int i = 0; i < 15; ++i){
        state[i] = state[i + 1];
        a15 ^= multiplicateInGaluaField(state[i], factors[i]);
    }
    state[15] = a15;
}

void performLTransformation(bytesVector &vec)
{
    for (int i = 0; i < 16; i++)
        performRTransformation(vec);
}

void performReverseLTransformation(bytesVector &vec)
{
    for (int i = 0; i < 16; i++)
        performReverseRTransformation(vec);
}


QVector<bytesVector> expandKey(QString &key)
{
    QVector<bytesVector> iterKeys;
    QVector<bytesVector> iterConstants;
    bytesVector C;
    QString hex;

    QString firstHalfOfKey = "";
    QString secondHalfOfKey = "";
    for(int i = 0; i < 32; ++i){
        firstHalfOfKey += key[i];
        secondHalfOfKey += key[i + 32];
    }

    bytesVector firstKey = convertHexStrToBytes(firstHalfOfKey);
    bytesVector secondKey = convertHexStrToBytes(secondHalfOfKey);
    iterKeys.push_back(firstKey);
    iterKeys.push_back(secondKey);

    for(int i = 0; i < 32; ++i){
        hex = QString("%1").arg((i + 1), 2, 16, QLatin1Char( '0' ));
        hex = hex.rightJustified(32, '0');
        C = convertHexStrToBytes(hex);
        performLTransformation(C);
        hex = convertBytesToHexStr(C);
        iterConstants.push_back(C);
    }

    bytesVector internal (16, 0x00);
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 8; ++j){
            performXTransformation(firstKey, iterConstants[i * 8 + j], internal);
            performSTransformation(internal);
            performLTransformation(internal);
            performXTransformation(internal, secondKey, C);
            secondKey = firstKey;
            firstKey = C;
        }
        iterKeys.push_back(firstKey);
        iterKeys.push_back(secondKey);
    }

    return iterKeys;
}


QString encryptBlock(QString &text, QString &key)
{
    QVector<bytesVector> keys = expandKey(key);
    bytesVector plainBytes = convertHexStrToBytes(text);
    bytesVector cipherBytes (16, 0x00);
    for(int i = 0; i < 9; ++i){
        performXTransformation(keys[i], plainBytes, cipherBytes);
        performSTransformation(cipherBytes);
        performLTransformation(cipherBytes);
        plainBytes = cipherBytes;
    }
    performXTransformation(keys[9], plainBytes, cipherBytes);
    QString ciphertext = convertBytesToHexStr(cipherBytes);
    return ciphertext;

}

QString decryptBlock(QString &text, QString &key)
{
    QVector<bytesVector> keys = expandKey(key);
    bytesVector cipherBytes = convertHexStrToBytes(text);
    bytesVector plainBytes (16, 0x00);
    performXTransformation(keys[9], cipherBytes, plainBytes);
    qDebug() << convertBytesToHexStr(plainBytes);
    for(int i = 8; i >= 0; --i){
        performReverseLTransformation(plainBytes);
        performReverseSTransformation(plainBytes);
        performXTransformation(keys[i], plainBytes, cipherBytes);
        plainBytes = cipherBytes;
    }

    QString plaintext = convertBytesToHexStr(plainBytes);
    return plaintext;

}

QString MSB(QString text, int n)
{
    QString res = "";
    for (int i = 0; i < n; ++i) {
        res += text[i];
    }
    return res;
}


QString LSB(QString text, int n)
{
    QString res = "";
    int start = text.length() - n;
    for (int i = start; i < text.length(); ++i) {
        res += text[i];
    }
    return res;
}

QVector<QString> getBlocks(QString str)
{
    QVector<QString> blocks;
    QString block = "";
    int blocksCount = floor(str.length() / 32);
    for (int i = 0; i < blocksCount; ++i) {
        for (int j = 0; j < 32; ++j) {
            block += str[i*32 + j];
        }
        blocks.push_back(block);
        block = "";
    }
    int index = blocksCount * 32;
    if (index < str.length()) {
        for (int ind = index; ind < str.length(); ++ind) {
            block += str[ind];
            index += 1;
        }
        blocks.push_back(block);
    }
    return blocks;
}



//не требуется дополнение текста
QString encryptKuznechikOFB(QString plaintext, QString key, QString IV)
{
    QVector<QString> blocks = getBlocks(plaintext);
    QString ciphertext = "";
    int k = blocks.size();
    //IV should be 64
    QString R = IV;
    QString P = "";
    QString gamma = "";
    bytesVector cipher (16, 0x00);
    bytesVector gam (16, 0x00);
    bytesVector text (16, 0x00);
    for (int i = 0; i < k; ++i) {

        P = blocks[i];

        gamma = MSB(R, 32);
        gamma = encryptBlock(gamma, key);
        gamma = MSB(gamma, P.length());
        gam = convertHexStrToBytes(gamma);
        text = convertHexStrToBytes(P);

        performXTransformation(text, gam, cipher);

        R = LSB(R, 32) + gamma;

        ciphertext += MSB(convertBytesToHexStr(cipher), P.length());
    }
    return ciphertext;
}

QString decryptKuznechikOFB(QString ciphertext, QString key, QString IV)
{
    QVector<QString> blocks = getBlocks(ciphertext);
    QString plaintext = "";

    int k = blocks.size();
    //IV should be 64
    QString R = IV;
    QString C = "";
    QString gamma = "";
    bytesVector plain (16, 0x00);
    bytesVector gam (16, 0x00);
    bytesVector text (16, 0x00);
    for (int i = 0; i < k; ++i) {

        C = blocks[i];

        gamma = MSB(R, 32);
        gamma = encryptBlock(gamma, key);
        gamma = MSB(gamma, C.length());
        gam = convertHexStrToBytes(gamma);
        text = convertHexStrToBytes(C);

        performXTransformation(text, gam, plain);
        R = LSB(R, 32) + gamma;
        plaintext += MSB(convertBytesToHexStr(plain), C.length());
    }
    return plaintext;
}

