# base64-algorithm
Este é um projeto que eu criei para entender o algoritmo do base64 e criar funções para codificar e decodificar dados à serem transmitidos em redes e armazena-los em banco de dados usando a linguagem C.

# Como funciona
O algoritmo base64 converte dados binarios (textos, imagens ou arquivos) em sequencia de bytes legiveis ```A-Z, a-z, 0-9, +, /``` e ```=``` para preenchimento (padding). Ele tranforma blocos de 3 bytes ```(3 x 8 = 24 bits)``` em 4 caracteres de 6 bits  ```(4 x 6 = 24 bits)```, isso faz com que o tamanho original aumente 33%. Exemplo:


```
ABC -> QUJD
```

Perceba que o valor final aumentou 1 byte, isso diz os 33% no tamanho final.

Bem, Como dito, o algoritmo converte blocos de 3 bytes em 4 caracteres de 6 bits, ou seja, ele não vai operar bytes individualmente, cada byte vai compartilhar seus bits entre si.

## Entrada
| A | B | C |
| :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 |

A primeira coisa que faremos é dividir este bloco de 8 bits em bloco de 6 bits:

| 16 | 20 | 9 | 3 |
| :---: | :---: | :---: | :---: |
| 010000 | 010100 | 001001 | 000011 |

O resultado foi 4 numeros que serão usados em uma lista de caracteres:

# Deslocamento de bits
As principais operações de deslocamento são ```>> (right shift)``` e ```<< (left shift)```, eles movem digitos binários para esquerda ou para direita. Por que deslocamento de bits?

| Caractere | Binário | Operador | Operando | Resultado |
| :---: | :---: | :---: | :---: | :---: |
| A | 0100 0001 | >> | 2 | 0001 0000 |

Dois bits foram movidos para a direita, descartando os 2 bits menos significativos. Dois bits 0 foram inseridos à esquerda. O resultado continua sendo um byte de 8 bits, mas agora apenas os 6 bits mais significativos originais foram mantidos.

```
01000001 -> 00010000
      ^^            ^^
```

# Operações bitwise
