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
Simples! Deslocando bits e combinando-os com operações bitwise

# Deslocamento de bits
As principais operações de deslocamento são ```>> (right shift)``` e ```<< (left shift)```, eles movem digitos binários para esquerda ou para direita. Exemplo:

| Caractere | Binário | Operador | Operando | Resultado |
| :---: | :---: | :---: | :---: | :---: |
| A | 01000001 | >> | 2 | 00010000 |

Dois bits foram movidos para a direita, descartando os 2 bits menos significativos. Dois bits 0 foram inseridos à esquerda. O resultado continua sendo um byte de 8 bits, mas agora apenas os 6 bits mais significativos originais foram mantidos.

```
01000001 -> 00010000
      ^^    ^^      ^^
```

Mas veja, isso ja codifica o primeiro byte para base64, o valor binário resultante da operação ```A >> 2``` é ```00010000``` convertido para inteiro ```16```, 

# Operações bitwise
