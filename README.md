##### *In case of derivative of divide operation is not available but others are finished.*


>Test inputs for section 1 and section 2 is in folder 'part1'
>
>Test inputs for section 3 and section 4 is in folder 'part2'


### Test command:
```bash
make 'or' make all  : test all program.

make expreval       : test section 1 expreval.c with all 'part1' test inputs.

make parsetree      : test section 2 parsetree.c with the first 4 'part1' test inputs.

make preinpost      : test section 3 preinpost.c with the first 4 'part1' test inputs.

make diff           : test section 3 diff.c with all 'part2' test inputs.

make simplify       : test section 4 simplify.c with all 'part2' test inputs.
