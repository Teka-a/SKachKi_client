#include "streebog.h"

void X(const bytesVector &vec1, const bytesVector &vec2, bytesVector &res)
{
    for (int i = 0; i < vec1.size(); ++i){
        res[i] = vec1[i] ^ vec2[i];
    }
}

void addInRingOFResidues(const bytesVector &vec1, const bytesVector &vec2, bytesVector &result)
{
    int internal = 0;
    for (int i = 0; i < 64; i++) {
        internal = vec1[i] + vec2[i] + (internal >> 8);
        result[i] = internal & 0xFF;
    }
}

void S(bytesVector &vec)
{
    for (int i = 0; i < 64; ++i){
        vec[i] = pi[vec[i]];
    }
}

void P(bytesVector &vec)
{
    bytesVector temp (64, 0x00);
    for (int i = 0; i < 64; ++i){
        temp[i] = vec[Tau[i]];
    }
    vec = temp;
}

void L(bytesVector &vec)
{
    QString hexStr = convertBytesToHexStr(vec);
    QString binStr = convertHexStrToBinStr(hexStr);
    QString chanks = "";
    bytesVector AElem;
    for (int i = 0; i < 8; ++i) {

        bytesVector temp (8, 0x00);
        for (int j = 0; j < 64; ++j) {
            //
            if (binStr[i*64+j] == "1"){
                AElem = convertHexStrToBytes(A[j]);
                X(temp, AElem, temp);
            }
        }
        chanks += convertBytesToHexStr(temp);

    }
    vec = convertHexStrToBytes(chanks);
}

QString padding(QString &textBin)
{
    QString padded = "";
    int textBinLength = textBin.length();
    int zeroCount = 511 - textBinLength;
    while (zeroCount != 0) {
        padded += "0";
        zeroCount--;
    }
    padded += "1";
    padded += textBin;
    return convertBinStrToHexStr(padded);
}

bytesVector g(bytesVector h, bytesVector m, bytesVector N)
{
    bytesVector K (64, 0x00);
    bytesVector nextK (64, 0x00);
    bytesVector res (64, 0x00);
    bytesVector prev (64, 0x00);

    X(h, N, K);
    S(K);
    P(K);
    L(K);
    prev = m;

    for (int i = 0; i < 12; ++i) {
        X(K, prev, res);
        S(res);
        P(res);
        L(res);
        prev = res;
        X(K, KeyC[i], nextK);
        S(nextK);
        P(nextK);
        L(nextK);

        K = nextK;
    }
    X(K, prev, res);
    X(res, h, res);
    X(res, m, res);

    return res;

}


QString getHash512(QString &text)
{
    //Step 1
    bytesVector h (64, 0x00);
    bytesVector N (64, 0x00);
    bytesVector sigma (64, 0x00);

    QString textBin = convertHexStrToBinStr(text);
    int textBinLength = textBin.length();


    //Step 3
    QString hexStr = padding(textBin);
    //3.1
    bytesVector m = convertHexStrToBytes(hexStr);
    //3.2
    h = g(h, m, N);
    //3.3
    QString hex = QString::number(textBinLength, 16);
    int zeroCount = 128 - hex.length();
    QString padded = "";
    while (zeroCount != 0) {
        padded += "0";
        zeroCount--;
    }
    padded += hex;
    bytesVector M = convertHexStrToBytes(padded);
    addInRingOFResidues(N, M, N);
    //3.4
    addInRingOFResidues(sigma, m, sigma);
    //3.5
    bytesVector zeroes (64, 0x00);
    h = g(h, N, zeroes);

    h = g(h, sigma, zeroes);
    return convertBytesToHexStr(h);
}
