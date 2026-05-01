# base64-algorithm
Este é um projeto que eu criei para entender o algoritmo do base64 e criar funções para codificar e decodificar dados à serem transmitidos em redes e armazena-los em banco de dados usando a linguagem C.

# Como funciona
O algoritmo base64 converte dados binarios (textos, imagens ou arquivos) em sequencia de bytes legiveis ```A-Z, a-z, 0-9, +, /``` e ```=``` para preenchimento (padding). Ele tranforma blocos de 3 bytes ```(3 x 8 = 24 bits)``` em 4 caracteres de 6 bits  ```(4 x 6 = 24 bits)```, isso faz com que o tamanho original aumente 33%. Exemplo:


```
ABC -> QUJD
```

Perceba que o valor final aumentou 1 byte, isso diz os 33% no tamanho final. Como dito, o algoritmo converte blocos de 3 bytes em 4 caracteres de 6 bits, ou seja, ele não vai operar bytes individualmente, cada byte vai combinar seus bits entre si.

## Entrada
| A | B | C |
| :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 |

Os três bytes (24 bits) são concatenados formando um fluxo contínuo de bits. Esse fluxo é então dividido sequencialmente em quatro grupos de 6 bits:

| 16 | 20 | 9 | 3 |
| :---: | :---: | :---: | :---: |
| 010000 | 010100 | 001001 | 000011 |

O resultado foi 4 numeros, cada um representando um índice na tabela Base64:

- A-Z: ```0-25```
- a-z: ```26-51```
- 0-9: ```52-61```
-   +: ```62```
-   /: ```63```

| Caractere | Índice |
| :---: | :---: |
| A | 0 |
| B | 1 |
| C | 2 |
| D | 3 |
| E | 4 |
| F | 5 |
| G | 6 |
| H | 7 |
| I | 8 |
| J | 9 |
| K | 10 |
| L | 11 |
| M | 12 |
| N | 13 |
| O | 14 |
| P | 15 |
| Q | 16 |
| R | 17 |
| S | 18 |
| T | 19 |
| U | 20 |
| ... | ... |

Se pegarmos os caracteres correspondente aos indices, temos:
- 16: ```Q```
- 20: ```U```
- 9: ```J```
- 3: ```D```

Resultado: ```QUJD```

OK! Mas como vou implementar esse algoritmo em c?
Simples, deslocando bits e combinando-os com operações bitwise

# Deslocando bits
As operações `>>` (right shift) e `<<` (left shift) movem bits para direita ou esquerda.. Exemplo:

| Caractere | Binário | Operador | Operando | Resultado |
| :---: | :---: | :---: | :---: | :---: |
| A | 01000001 | >> | 2 | 00010000 |

Dois bits foram movidos para a direita, descartando os 2 bits menos significativos. Dois bits 0 foram inseridos à esquerda. O resultado continua sendo um byte de 8 bits, mas agora apenas os 6 bits mais significativos originais foram mantidos.

```
01000001 -> 00010000
      ^^    ^^      ^^
```

Isso ja codifica o primeiro byte para base64, o valor binário resultante da operação ```A >> 2``` é ```00010000``` convertido para inteiro ```16``` na tabela base64 ```Q``` só que 2 bits ```01``` foram descartados e eles são necessários para concatenar com 4 bits mais significativos do segundo byte ```B``` resultando no segundo grupo de 6 bits:

### 1º grupo
Seis primeiros bits do byte A (descartando 2 bits)

```
A = 010000|01|
01000001 >> 2 = 00010000 (01 descartados)
```

### 2º grupo
Dois bits descartados do byte A + 4 primeiros bits do byte B (descartando 4 bits)

```
B =  0100|0010|
01 + 0100 = 010100 (0010 descartados)
```

### 3º grupo
Quatro bits descartados do byte B + 2 primeiros bits do byte C (descartando 6 bits)

```
C = 01|000011|
0010 + 01 = 001001 (000011 descartados)
```

### 4º grupo
Seis bits descartados do byte C

```
000011 (novo indice da tabela base64)
```

# Obtendo bits específicos
Para obter bits especificos do byte ```A``` por exemplo, precisamos utilizar a **operação bitwise AND (&)** que fará uma comparação bit a bit de um número com outro, o bit resultante é ```1``` se ambos os bits correspondentes forem ```1```, caso contrário, é ```0```. Exemplo:

```
A = 01000001
A & 00000011
    00000001
```

Usamos ```00000011``` para obter 2 ultimos bits do byte ```A```, o resultado foi ```00000001``` porque o único bit que corresponde a ```1``` entre os dois números é o ultimo bit, para obter os 4 ultimos bits usamos ```00001111```:

```
C = 01000011
C & 00001111
    00000011
```

# Concatenando bits
Para concatenar os bits vamos utilizar **operação bitwise OR (|)** combinado com **deslocamento de bits** abordado [anteriormente](#deslocando-bits), primeiro precisamos saber quanto de bits vamos concatenar, 


```
```
