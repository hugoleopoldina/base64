# base64-algorithm
Este é um projeto que eu criei para entender o algoritmo do base64 e criar funções para codificar e decodificar dados à serem transmitidos em redes e armazena-los em banco de dados usando a linguagem C.

# Como funciona
O algoritmo base64 converte dados binarios (textos, imagens ou arquivos) em sequência de caracteres ASCII legíveis ```A-Z, a-z, 0-9, +, /``` e ```=``` para preenchimento (padding). Ele tranforma blocos de 3 bytes ```(3 x 8 = 24 bits)``` em 4 caracteres de 6 bits  ```(4 x 6 = 24 bits)```, isso faz com que o tamanho original aumente 33%. Exemplo:


```
ABC -> QUJD
```

Perceba que o valor final aumentou 1 byte, isso diz os 33% no tamanho final. O algoritmo não vai operar bytes individualmente, os bytes são processados em blocos de 3, combinando seus bits entre si.

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

| Caractere | Índice |
| :---: | :---: |
| Q | 16 |
| U | 20 |
| J | 9 |
| D | 3 |

Resultado: ```QUJD```

# Se o tamanho da entrada não for múltiplo de 3?
A divisão dos grupos de 3 bytes quebram e sobram bytes a serem codificados. Exemplo:

| A | B | C | D | 
| :---: | :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 | 01000100 |

Dividindo em blocos de 3 bytes:

| A | B | C |
| :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 |

Veja que sobrou o byte ```D```

| D |
| :---: |
| 01000100 |



# Como calcular o tamanho final?
Primeiro dividimos o tamanho total da entrada em blocos de 3:

```
        Entrada: ABC
Total de blocos: 3 / 3 = 1
```

Tivemos o resultado de 1 bloco de ```24 bits```, a cada ```24 bits``` o algoritmo gera ```4 caracteres``` da tabela base64, então temos o seguinte calculo:

```
      Entrada: ABC
Tamanho final: 3 / 3 * 4 = 4
```

Feito! O tamanho final é de ```4 bytes``` mas tem um porém, e se o tamanho de entrada não for ```múltiplo de 3```? Aí a divisão dos blocos quebram e temos que arredondar o tamanho final para ```múltiplo de 4```. Exemplo:

```
      Entrada: ABCD
Tamanho final: 4 / 3 * 4 = 5.33333333
```

O múltiplo de 4 mais próximo é o ```8```, faremos o arredondamento para cima do tamanho final quebrado ```5.33333333```

```
      Entrada: ABCD
Tamanho final: ceil(4 / 3 * 4) = 6
```

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
Seis primeiros bits do byte ```A``` (descartando 2 bits)

```
A = |010000|01|
01000001 >> 2 = 00010000 (01 descartados)
```

### 2º grupo
Dois bits descartados do byte ```A``` + 4 primeiros bits do byte ```B``` (descartando 4 bits)

```
B = |0100|0010|
01 + 0100 = 010100 (0010 descartados)
```

### 3º grupo
Quatro bits descartados do byte ```B``` + 2 primeiros bits do byte ```C``` (descartando 6 bits)

```
C = |01|000011|
0010 + 01 = 001001 (000011 descartados)
```

### 4º grupo
Seis bits descartados do byte ```C```

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
Para concatenar os bits vamos utilizar **operação bitwise OR (|)** combinado com **deslocamento de bits** abordado [anteriormente](#deslocando-bits)

| A | B | C |
| :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 |

| 16 | 20 | 9 | 3 |
| :---: | :---: | :---: | :---: |
| 010000 | 010100 | 001001 | 000011 |

### 1° grupo

Basta deslocar dois bits para direita ```A >> 2``` resultado ```010000```

### 2° grupo
Dois ultimos bits do byte ```A``` concatendos com os 4 primeiros bits do byte ```B```

```
A & 00000011 = 01
      B >> 4 = 0100
```

Pegue os 2 ultimos bits do byte ```A``` e abra espaço para 4 bits, deslocando para esquerda:

```
01 << 4 = 010000
```

Em seguida use a operação ```OR (|)``` para juntar os 4 primeiros bits do byte ```B```

```
010000 | 0100 = 010100
```

O resultado juntando os cálculos ficaria:

```
((A & 00000011) << 4) | (B >> 4 ) = 010100
```

### 3º grupo
Quatro ultimos bits do byte ```B``` concatenados com 2 primeiros bits do byte ```C```

```
((B & 00001111) << 2) | (C >> 6) = 001001
```

### 4º grupo
Seis ultimos bits do byte ```C```

```
C & 00111111 = 000011
```

Essa é a lógica que utilizei para codificar dados para base64 em C
