# ZefLogger

* [Types index](Developer/ClassDescription/typesIndex.md)
* [Way Of Working with docs](WayOfWorking.md)

## Menu

* Developer
* * [Class Description](Developer/ClassDescription/index.md)
* * [System Description](Developer/SystemDescription/index.md)
* User
* * [User Guide](User/Guide/index.md)
* * [User Manual](User/Manual/index.md)


## Commands

* `mkdocs new [dir-name]` - Create a new project.
* `mkdocs serve` - Start the live-reloading docs server.
* `mkdocs build` - Build the documentation site.
* `mkdocs -h` - Print help message and exit.

## Project layout

    mkdocs.yml    # The configuration file.
    docs/
        index.md  # The documentation homepage.
        ...       # Other markdown pages, images and other files.


### some page

```plantuml
participant Alice
actor Bob

ref over Alice, Bob : Please see the [[/Developer/ClassDescription/_global/Class.HelloWorld/#virtual-stdstring-gethelloworld-const-override That link]] for further details. - go and check

Alice -> Bob : hello
note left [[http://plantuml.com/start]]
  a note with a link
end note
ref over Bob
  This can be on
  several lines
end ref
```
