## INFO ##
## INFO ##

# Import python modules
from sys import argv

# Examples
START = 1
COUNT = 3

# Tupfile rules
TO_OBJ = """\
: foreach {DIR}/{FILE}-{I:0>2}/*.c {DIR}/{FILE}-{I:0>2}/src/*.c | {{{T}_lib}}  \
    |> !to_obj $(CPPPATH) -I{DIR}/{FILE}-{I:0>2}/include                       \
    |> $(BUILD_DIR)/{DIR}/{FILE}-{I:0>2}/%B.o {{{T}_bin{I}}}"""

TO_BIN = """\
: {{{T}_bin{I}}} | {{{T}_lib}}                                                 \
    |> !to_bin -L$(LIBRARY_DIR) -lmeaculpa.safe                                \
    |> $(BINARY_DIR)/{FILE}-{I:0>2}"""

# Print build rules
ARGS = {'DIR': argv[1], 'FILE': argv[2], 'T': argv[3]}
for i in range(START, COUNT + 1):
    print(TO_OBJ.format(I=i, **ARGS))
    print(TO_BIN.format(I=i, **ARGS))
