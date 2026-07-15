# Graphoni

## WARNING
TODO: This readme is not complete, there's missing stuff, especially in the
[usage](#usage) section.

## Usage

### Run Tests

```console
python -m test.test
```

### Use Library/Program(?)

TODO: choose between wether this is a library offering functions, or a program
expecting arguments.

IF LIB: You can add this library in your projects using the path to MAIN FILE.

IF PROG: You can execute MAIN FILE with the input and the output file as arguments.

## Description

This is Joachim Lardinois and Aleksandr Pavlov's code and report for our
"Théorie des graphes" project.

We choose to answer question 2, the minimum spanning tree.
So we take as input a file describing a graph, and output a MST of this graph.
We were also asked to manage to get another different MST for the same graph,
and finally, be able to dynamically update our MST while we're adding edges and
nodes to the graph.

## Learn More

The pdf file "main.pdf" in the "report" folder is a deeper dive into our research
and methods.

## Authors

This project was made by Joachim Lardinois and Aleksandr Pavlov, students at ULG,
and was made as an assignment for one of our lessons.

## References

You can find inside of the "docs" folder all our resources used for this project.
If you see a number between brackets in a text (ex: "blabla (02) bla blabla"), it
is a reference to the corresponding document in "docs". And if the number is
preceded by the letter 'l', it means it's referencing a link in "links_and_other.txt"

## Folders

The docs folder contains all our documentation, links, and other where we got
the algos we used.

The report folder contains the report pdf, as well as all the tex files used in
compiling that pdf.

The src folder contains all the python code files used in the project.
TODO: if you already had python files, please move them to "src/..."

## TODO

- [ ] Code
  - [x] CLI
  - [x] Read file
    - [x] Possible Errors
  - [ ] MST
    - [ ] Search for docs and algos
      - [ ] Prim's algorithm implemented
      - [ ] Kruskal's algorithm implemented
      - [ ] Borůvka's algorithm implemented
      - [x] Reverse-delete algorithm implemented
    - [ ] Possible Errors (ex: not connexe)
    - [ ] Get Other MST
    - [x] Solution visualization
    - [x] Step by step visualization
  - [ ] Dynamic Update
    - [x] Commands for changing the graph
    - [ ] Search for docs and algos
    - [ ] Possible Errors (ex: not connexe)

- [ ] Report
  - [ ] Explain MST
    - Need more info once code is done
  - [ ] Explain Other MST
    - Need more info once code is done
  - [ ] Explain Dynamic Update
    - Need more info once code is done
  - [ ] Compile all docs searched for references

The 3 points under Code (Read/MST/Dyn) can all be coded separately from each other.
If you've already done some of those, don't hesitate to tell me and update this
file by crossing off the ones already done.

## Deadline

T-8days
