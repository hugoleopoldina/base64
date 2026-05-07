# base64-algorithm
Este projeto foi criado para estudar o funcionamento do algoritmo Base64 e implementar funções de codificação e decodificação em linguagem C. O objetivo é entender como dados podem ser transformados para transmissão em redes ou armazenamento em sistemas que aceitam apenas caracteres ASCII.

# Como funciona
O Base64 converte dados binários (textos, imagens ou arquivos) em uma sequência de caracteres ASCII legíveis:

```
A-Z, a-z, 0-9, +, /
```

O caractere ```=``` é utilizado como **padding** (preenchimento).

O algoritmo transforma blocos de **3 bytes (3 × 8 = 24 bits)** em 4 caracteres, onde cada caractere representa **6 bits (4 × 6 = 24 bits)**. Isso resulta em um aumento de aproximadamente **33% no tamanho dos dados**.

Exemplo:
```
ABC -> QUJD
```

O algoritmo não processa bytes individualmente, mas sim em blocos de 3 bytes, combinando seus bits.

# Transformação passo a passo
| A | B | C |
| :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 |

Os 3 bytes são concatenados formando um fluxo contínuo de 24 bits. Esse fluxo é então dividido sequencialmente em 4 grupos de 6 bits:

| 16 | 20 | 9 | 3 |
| :---: | :---: | :---: | :---: |
| 010000 | 010100 | 001001 | 000011 |

Cada valor representa um índice na tabela Base64:

- A-Z: ```0-25```
- a-z: ```26-51```
- 0-9: ```52-61```
-   +: ```62```
-   /: ```63```

Convertendo os índices:

| Caractere | Índice |
| :---: | :---: |
| Q | 16 |
| U | 20 |
| J | 9 |
| D | 3 |

Resultado:

```
QUJD
```

# Quando o tamanho não é múltiplo de 3
Se a entrada não for múltiplo de 3 bytes, o último bloco fica incompleto.

Exemplo:
| A | B | C | D | 
| :---: | :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 | 01000100 |

O bloco ```ABC``` é processado normalmente. O byte restante (```D```) precisa ser tratado.

| D |
| :---: |
| 01000100 |

Para completar 24 bits, adicionamos zeros à direita:

| D | padding | padding |
| :---: | :---: | :---: |
| 01000100 | 00000000 | 00000000 |

Em seguida dividimos em 4 blocos de 6 bits

| 17 | 0 | padding | padding |
| :---: | :---: | :---: | :---: |
| 010001 | 000000 | 000000 | 000000 |

O primeiro grupo corresponde aos 6 primeiros bits do byte ```D```, sobrando 2 bits e esses 2 bits foram preenchidos com 4 bits à direita, criando o segundo grupo de 6 bits e os últimos 2 grupos não representam dados reais, apenas indicam que houve preenchimento (padding) para completar um bloco de 4 caracteres.

Convertendo os índices:

| Caractere | Índice |
| :---: | :---: |
| R | 17 |
| A | 0 |
| = | 0 |
| = | 0 |

Resultado:

```
QUJDRA==
```

# Cálculo do tamanho final
Primeiro dividimos o tamanho total da entrada em blocos de 3:

```
        Entrada = ABC
Total de blocos = 3 / 3 = 1
```

Tivemos o resultado de 1 bloco de ```24 bits```, a cada ```24 bits``` o algoritmo gera ```4 caracteres``` da tabela base64, então temos o seguinte calculo:

```
      Entrada = ABC
Tamanho final = 3 / 3 * 4 = 4
```

O tamanho final é de ```4 bytes``` mas se o tamanho de entrada não for ```múltiplo de 3``` a divisão dos blocos quebram e temos que arredondar o tamanho para o próximo múltiplo de 4.

Exemplo:
```
      Entrada = ABCD
Tamanho final = 4 / 3 * 4 = 5.33333333
```

O múltiplo de 4 mais próximo é o ```8```, faremos o arredondamento para cima do tamanho final quebrado ```5.33333333```:

```
      Entrada = ABCD
Tamanho final = ceil(4 / 3) * 4 = 8
```

# Implementação em C (bitwise)
A codificação pode ser feita utilizando operações bitwise (```>>```, ```<<```, ```&```, ```|```) para extrair e combinar bits.

## Deslocando bits

As operações:
- ```>>``` (right shift): desloca bits para a direita
- ```<<``` (left shift): desloca bits para a esquerda

Exemplo:
| Caractere | Binário | Operação | Resultado |
| :---: | :---: | :---: | :---: |
| A | 01000001 | A >>  2 | 00010000 |

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

## Obtendo bits específicos
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

## Concatenando bits
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

# Decodificação
Da mesma forma que a cooficação transforma 3 bytes em 4 caracteres, a decodificação vai tranformar 4 caracteres em 3 bytes, agrupando 4 grupos de 6 bits em 3 grupos de 8 bits

Exemplo:
| 16 | 20 | 9 | 3 |
| :---: | :---: | :---: | :---: |
| Q | U | J | D |
| 010000 | 010100 | 001001 | 000011 |

Agrupando em grupos de 8 bits:

| A | B | C |
| :---: | :---: | :---: |
| 01000001 | 01000010 | 01000011 |

## Transformação passo a passo
### 1º grupo
Seis bits do 1º grupo + 2 primeiros bits do 2º grupo:

```
1º grupo: 010000
2º grupo: 010100
```

Seis bits do 1º grupo:

```
010000
```

Abrir espaço para mais 2 bits:

```
010000 << 2 = 01000000
```

Dois primeiros bits do 2º grupo:

```
010100 >> 4 = 01
```

Concatenar os resultados:

```
01000000 | 01 = 01000001
```

Cálculo final:

```
(010000 << 2) | (010100 >> 4)
```

### 2º grupo
Quatro últimos bits do 2º grupo + 4 primeiros bits do 3º grupo:

```
2º grupo: 010100
3º grupo: 001001
```

Quatro ultimos bits do 2º grupo:

```
010100 & 001111 = 0100
```

Abrir espaço para mais 4 bits

```
0100 << 4 = 01000000
```

Quatro primeiros bits do 3º grupo:

```
001001 >> 2 = 0010
```

Concatenar os resultados:

```
01000000 | 0010 = 01000010
```

Cálculo final:

```
((010100 & 001111) << 2) | (001001 >> 2) = 01000010
```

### 3º grupo
Dois últimos bits do 3º grupo + 6 bits do 4º grupo:

```
3º grupo: 001001
4º grupo: 000011
```

Dois últimos bits do 3º grupo:

```
001001 & 000011 = 01
```

Abrir espaço para mais 6 bits:

```
01 << 6 = 01000000
```

Seis bits do 4º grupo:

```
000011
```

Concatenar os resultados:

```
01000000 | 000011 = 01000011
```

Cálculo final:

```
((001001 & 000011) << 6) | 000011 = 01000011
```

# Conclusão
O Base64 é um algoritmo simples, mas poderoso, que permite representar dados binários em formato textual. Sua implementação exige compreensão de manipulação de bits, especialmente operações de deslocamento e máscaras.
