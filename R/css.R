
not_empty <- function(x){
  x[ x != ""]
}

tokenize_css <- function(txt) .Call(tokenize_css_impl, txt)

#' read a css file into a tibble
#'
#' @param file css file to parse, read with [readr::read_file()] so can be a url
#' @param ... `%>%` [readr::read_file()]
#'
#' @return a tibble with columns:
#' - rule: the name of the css rule e.g. `.some-css-class`
#' - property: the name of a property, e.g. `background-image`
#' - value: value of the setting, e.g. `1px solid black`
#'
#' @importFrom readr read_file
#' @importFrom magrittr %>%
#' @export
read_css <- function(file, ...){
  txt <- read_file(file, ...) %>%
    gsub( "/[*].*?[*]/", "", . ) %>%
    gsub( "//.?\n", "\n", . ) %>%
    tokenize_css()
}


#' write css tibble into file
#'
#' @param data css tibble, e.g. generate by [read_css()]
#' @param path file path
#' @param ... passed to [readr::write_lines()]
#'
#' @importFrom glue glue
#' @importFrom dplyr mutate group_by summarise pull
#' @importFrom readr write_lines
#' @export
write_css <- function(data, path, ...){
  data %>%
    mutate( text = glue("{property}: {value} ; ") ) %>%
    group_by(rule) %>%
    summarise( text = paste(paste("  ", text ), collapse = "\n") ) %>%
    mutate( glue(
      "{rule} {{ \n{text} \n }}\n"
    ) ) %>%
    pull() %>%
    write_lines(path, ...)
}


#' @importFrom utils globalVariables
globalVariables(c("data", "rule", "property", "value", ".", "text"))

