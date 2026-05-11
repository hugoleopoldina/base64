/*
* @file base64.c
* @brief Funções para codificação e decodificação Base64.
*
* @author Hugo Leopoldina
* @date 2026-05-08
* @version 1.0.0
*/

#include "base64.h"

// tabela ASCII
static const char base64_table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/+";

char* base64_encode(unsigned char* bytes, size_t size) {
    // tamanho final truncado
    size_t total = (size_t)trunc(ceil((float)size / 3) * 4);

    // resultado
    char *result = (char*)malloc(total + 1),
          stream[3] = {0}; // operar cada 3 bytes
                                                                                                                                                                                                               
    if (!result) {
        return NULL;
    }

    // zerar result
    memset(result, 0, total + 1);

    int stream_index = 0, // usado para percorrer stream
        result_index = 0; // usado para percorrer result

    // percorrer os bytes
    for (size_t i = 0; i < size; i++) {
        // até que defina 3 bytes em stream
        stream[stream_index++] = bytes[i];
        if (stream_index == 3) {
            // definindo o byte codificado em result e incrementando indice
            // primeiro grupo; 6 primeiros bits do 1º byte
            result[result_index++] = base64_table[stream[0] >> 2];

            // segundo grupo; 2 ultimos bits do 1º byte + 4 primeiros bits do 2º byte
            result[result_index++] = base64_table[((stream[0] & 0x3) << 4) | (stream[1] >> 4)];

            // terceiro grupo; 4 ultimos bits do 2º byte + 2 primeiros bits do 3º byte
            result[result_index++] = base64_table[((stream[1] & 0xF) << 2) | (stream[2] >> 6)];

            // quarto grupo; 6 ultimos bits do 3º byte
            result[result_index++] = base64_table[stream[2] & 0x3F];

            // resetar stream
            memset(stream, 0, 3);
            stream_index = 0;
        }
    }

    // tratar os bytes restantes
    // pode sobrar 1 ou 2 bytes
    if (stream_index != 0) {
        // o 1º grupo sempre é os 6 primeiros bits do 1º byte
        result[result_index++] = base64_table[stream[0] >> 2];

        // se sobrar 1 byte, +1 caractere +2 padding
        if (stream_index == 1) {
            // 2º grupo; 2 ultimos bits do 1º byte + 4 zeros
            result[result_index++] = base64_table[(stream[0] & 0x3) << 4];

            // (3º e 4º grupo)
            result[result_index++] = '=';
            result[result_index++] = '=';
        }

        // se sobrar 2 bytes, +2 caractere é gerado +1 padding
        else if (stream_index == 2) {
            // 2º grupo; 2 ultimos bits do 1º byte + 4 primeiros bits do 2º byte
            result[result_index++] = base64_table[((stream[0] & 0x3) << 4) | stream[1] >> 4];

            // 3º grupo; 4 ultimos bits do 2º byte + 2 zeros
            result[result_index++] = base64_table[(stream[1] & 0xF) << 2];

            // 4º grupo; preenchimento (padding)
            result[result_index++] = '=';
        }
    }

    return result;
}

unsigned char* base64_decode(char* encoded, size_t* out_size) {
    // calcular tamanho sem preenchimento
    int enc_len = strlen(encoded),
        padding = 0,
        total   = 0;

    if (encoded[enc_len - 1] == '=') padding++;
    if (encoded[enc_len - 2] == '=') padding++;

    total = ((enc_len / 4) * 3) - padding;

    // resultado
    unsigned char* result = (unsigned char*)malloc(total);

    if (!result) {
        return NULL;
    }

    // zerar bloco result
    memset(result, 0, total);

    // operar 4 numero de 8 bits que corresponde a um indice da tabela
    uint8_t stream[4] = { 0 };

    int stream_index = 0, // para percorrer stream
        result_index = 0; // para percorrer result

    // percorrer cada caractere codificado
    for (int i = 0; i < enc_len; i++) {
        char target = encoded[i];

        if (target == '\n' || target == '\r')
            continue;

        uint8_t index = 0;

        // até que encontre o indice correspondente
        // index 0 se o caractere for de padding
        if (target != '=') {
            while (base64_table[index] != target) {
                index++;
            }
        }

        // salvar o indice que corresponde ao caractere codificado em stream
        stream[stream_index++] = index;

        // até definir 4 indices em stream
        if (stream_index == 4) {
            // 1º grupo de 8 bits
            // 6 bits do 1º indice + 2 primeiros bits do 2º indice
            result[result_index++] = (stream[0] << 2) | (stream[1] >> 4);

            // 2º grupo de 8 bits
            // 4 ultimos bits do 2º indice + 4 primeiros bits do 3º indice
            if (result_index < total) // verificar se padding
                result[result_index++] = ((stream[1] & 0xF) << 4) | (stream[2] >> 2);
            

            // 3º grupo de 8 bits
            // 2 ultimos bits do 3º indice + 6 bits do 4º indice
            if (result_index < total) // verificar se padding
                result[result_index++] = ((stream[2] & 0x3) << 6) | stream[3];

            // zerar
            memset(stream, 0, 4);
            stream_index = 0;
        }
    }

    // escrever tamanho total se out_size definido
    if (out_size)
        *out_size = total;

    return result;
}
