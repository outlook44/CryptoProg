#include <iostream>
#include <string>
#include <cstring>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/sha.h>

void DeriveKeyAndIV(const std::string& password, CryptoPP::byte key[CryptoPP::AES::BLOCKSIZE], CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE]) {
    CryptoPP::SHA224 hash;
    hash.Update((const CryptoPP::byte*)password.data(), password.size());
    hash.Final(key);
    std::memcpy(iv, key, CryptoPP::AES::BLOCKSIZE);
}

void EncryptFile(const std::string& inputFilename, const std::string& outputFilename, const std::string& password) {
    CryptoPP::byte key[CryptoPP::AES::BLOCKSIZE];
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
    DeriveKeyAndIV(password, key, iv);

    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(key, sizeof(key), iv);

        CryptoPP::FileSource fs(inputFilename.c_str(), true,
            new CryptoPP::StreamTransformationFilter(encryptor,
                new CryptoPP::FileSink(outputFilename.c_str())
            )
        );
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Ошибка шифрования: " << e.what() << std::endl;
        exit(1);
    }
}

void DecryptFile(const std::string& inputFilename, const std::string& outputFilename, const std::string& password) {
    CryptoPP::byte key[CryptoPP::AES::BLOCKSIZE];
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
    DeriveKeyAndIV(password, key, iv);

    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor;
        decryptor.SetKeyWithIV(key, sizeof(key), iv);

        CryptoPP::FileSource fs(inputFilename.c_str(), true,
            new CryptoPP::StreamTransformationFilter(decryptor,
                new CryptoPP::FileSink(outputFilename.c_str())
            )
        );
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Ошибка расшифровки: " << e.what() << std::endl;
        exit(1);
    }
}

void ShowHelp(const char* progName) {
    std::cout << "  " << progName << " -e <вход> <выход> <пароль>  — зашифровать\n"
              << "  " << progName << " -d <вход> <выход> <пароль>  — расшифровать\n"
              << "  " << progName << " -h                           —справка\n\n";
}

int main(int argc, char* argv[]) {
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        ShowHelp(argv[0]);
        return 0;
    }

    if (argc != 5) {
        std::cerr << "Ошибка: неверное количество аргументов.\n";
        ShowHelp(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    std::string password = argv[4];

    if (mode == "-e") {
        EncryptFile(inputFile, outputFile, password);
        std::cout << "Файл успешно зашифрован.\n";
    } else if (mode == "-d") {
        DecryptFile(inputFile, outputFile, password);
        std::cout << "Файл успешно расшифрован.\n";
    } else {
        std::cerr << "Ошибка: неизвестный режим '" << mode << "'. Используйте -e, -d или -h.\n";
        return 1;
    }

    return 0;
}