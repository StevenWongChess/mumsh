##### MileStone1

###### 1, read/parse/execute loop & `exit`

```bash
# test 1 (print exit and exit)
exit
# test 2 (shoule behave same as exit)
exit randomthing
```

###### 2, & 3, execute command with/without args

```bash
# test 1
ls
# test 2
ls -a
# test 3 (will output error)
diff
```

###### 4, redirection (> >> and <)

```bash
# test 1 (> will override)
echo 123 > 1.txt
echo 321 > 1.txt
cat 1.txt
# test 2 (> will append)
echo 123 > 1.txt
echo 321 >> 1.txt
cat 1.txt
# test 3
echo 123 > 1.txt
cat < 1.txt
# test 4 (combine >/>> and <)
cat < 1.txt >> 2.txt
```

###### 5, bash style  (> >> and < can be anywhere)

```bash
# test 1 (multi space ignored)
echo 123 abc
echo 123  abc 
# test 2 (anywhere)
< 1.txt > 2.txt cat
< 1.txt cat > 2.txt
# test 3 (no need for space)
<1.txt >2.txt cat
<1.txt cat>2.txt
```



##### MileStone2

###### 6, pipe

```bash
# test 1
ls | grep mum
# test 2 (parallel, set time limit 1s)
sleep 1 | sleep 1 | sleep 1 | sleep 1 | sleep 1
# test 3 (tricky)
echo exit | ./mumsh
# test 4 (work with bash\\\\\\\\\\\\\\
cat < 1.txt | cat > 2.txt
# test 5 (bash style)
cat < 1.txt|grep 12
# test 6 (cascade pipes)
cat < 1.txt | cat | cat | cat | cat >> 2.txt
```

###### 7, `Ctrl-D`

```bash
# test 1
echo 12ctrl-d3 ## will output 123
# test 2
ctrl-d ## exit 
```

###### 8, Built-in `pwd`, `cd` 

```bash
# test 1
pwd
# test 2
pwd | cat > 1.txt
# test 3
cd .
cd ..
cd ~
cd -
cd /dir
cd dir
```

###### 9, `Ctrl-C`

```bash
# test 1 (pipe)
sleep 10 # then ctrl-C
# test 2 (multi pipe)
sleep 10 | sleep 10 | sleep 10 | sleep 10 | echo 123 # then ctrl-c
# test 3 (incomplete input)
echo incomplet # then ctrl-c before enter
# empty line then ctrk-c

```

###### 10, quotes

```bash
# test 1 (program name can also be quoted)
"echo" 'abc def' "ee""ff"
# test 2 won't matter because of the token logic will not affect redirections
"echo" "<1.'txt'" < 1.txt > "2.""txt"
# test 3 
echo "abc
def"
```

###### 11, multi-line

```bash
# test 1
echo 123>
1.txt
```

###### 12, error handling

```bash
1. Non-existing program
• input: non-exist abc def
• input: echo abc | non-exist
• output: non-exist: command not found
2. Non-existing file in input redirection 
• input: cat < non-existing.txt
• output: non-existing.txt: No such file or directory 
3. Failed to open file in output redirection
• input: echo abc > /dev/permission_denied
• output: /dev/permission_denied: Permission denied
4. Duplicated input redirection
• input: echo abc < 1.txt < 2.txt
• input: echo abc | grep abc < 1.txt
• output: error: duplicated input redirection
5. Duplicated output redirection
• input: echo abc > 1.txt > 2.txt
• input: echo abc > 1.txt >> 2.txt
• input: echo abc > 1.txt | grep abc
• output: error: duplicated output redirection
6. Syntax Error
• input:echo abc > > > >
• output: syntax error near unexpected token `>' 
• input: echo abc > < 1.txt
• output: syntax error near unexpected token `<' 
• input: echo abc > | grep abc
• output: syntax error near unexpected token `|'
7. Missing program
• input: > abc | | grep 123
• output: error: missing program 
8. cd to non-existing directory
• input: cd non-existing
• output: non-existing: No such file or directory
```





















