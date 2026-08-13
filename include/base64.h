/*
* @file base64.h
* @brief Funções para codificação e decodificação Base64.
*
* @author Hugo Leopoldina
* @date 2026-05-08
* @version 1.0.0
*/

#ifndef _BASE64_
#define _BASE64_

#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <math.h>

/*
* @brief Codifica bloco de bytes em carcateres ASCII
* @param bytes bloco a ser codificado
* @param size tamanho do bloco
* @return string ASCII codificado
* @warning o retorno deve ser liberado usando free()
*/
char* base64_encode(unsigned char* bytes, size_t size);

/*
* @brief Decodifica uma string Base64
* @param encoded string Base64 a ser decodificado
* @param out_size se definido, o tamanho dos dados seram escritos
* @return bloco de bytes decodificados
* @warning o retorno deve ser liberado usando free()
*/
unsigned char* base64_decode(char* encoded, size_t* out_size);

#endif
