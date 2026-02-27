# Way Of Working

## Example of using variables:

1. On top of a md page:

    ```
    ---
    title: Page title
    authors:
        - List Of Authors
    date: 2000-01-01
    url: https://composer.zefir.tech
    variableName: path/to/folder/
    ---
    ```

2. Later in that file

    ```html
    {% raw %}
    {% include variableName ~ "SomeFile1.md" %}
    {% include variableName ~ "SomeFile2.md" %}
    {% endraw %}
    ```
