# base64-algorithm
Este é um projeto que eu criei para entender o algoritmo do base64 e criar funções para codificar e decodificar dados à serem transmitidos em redes e armazena-los em banco de dados usando a linguagem C.

# Como funciona
O algoritmo base64 converte dados binarios (textos, imagens ou arquivos) em sequencia de bytes legiveis ```A-Z, a-z, 0-9, +, /``` e ```=``` para preenchimento (padding). Ele tranforma blocos de 3 bytes ```(3 x 8 = 24 bits)``` em 4 caracteres de 6 bits  ```(4 x 6 = 24 bits)```, isso faz com que o tamanho original aumente 33%.

# Desafios
+ [Deslocamento de bits](#deslocamento-de-bits)
+ [Operações bitwise](#operações-bitwise)

# Deslocamento de bits
As principais operações de deslocamento são ```>> (right shift)``` e ```<< (left shift)```, eles movem digitos binários para esquerda ou para direita. Por que deslocamento de bits? Como dito, o algoritmo converte blocos de 3 bytes em 4 caracteres de 6 bits, ou seja, ele não vai operar bytes individualmente, cada byte vai compartilhar seus bits entre si. Exemplo:

| Caractere | Binário | Operador | Operando | Resultado |
| :---: | :---: | :---: | :---: | :---: |
| A | 0100 0001 | >> | 2 | 0001 0000 |

O que aconteceu? Simples, 2 bits foram movidos para a direita e 2 bits (0s) foram adicionados à esquerda

# Operações bitwise
