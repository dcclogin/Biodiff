# Biodiff

Just a simple bioinformatics tool.

## Usage

There are two modes to compare two files containing bioinformatics data

1. Coordinate-based diff
2. Name-based diff

### Coordinate-based diff

> A set of coordinates are selected to see how much the DNA seqs in two files overlap

example:

```
~$ ./biodiff -c -a 4,5 -b 4,5 fileA fileB
```

- It selects the 4th and 5th columns in fileA, which represent for a DNA seq, and the same in fileB.
- Then the two files will be compared according to the two columns.

### Name-based diff

> One column(name) in each file are selected to see how many the gene names in two files are the same

example:

```
~$ ./biodiff -n -a 9 -b 9 fileA fileB
```

- It selects the 9th column in fileA, which represents for a name of DNA, and the same in fileB.
- Then the two files will be compared according to the certain column.  
