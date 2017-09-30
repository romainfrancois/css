
not_empty <- function(x){
  x[ x != ""]
}

#' read a css file into a tibble
#'
#' @param file css file to parse, read with [readr::read_file()] so can be a url
#' @param ... `%>%` [readr::read_file()]
#'
#' @return a tibble with columns
#' - rule: the name of the css rule e.g. `.some-css-class`
#' - setting: the name of a setting, e.g. `background-image`
#' - value: value of the setting, e.g. `1px solid black`
#'
#' @importFrom stringr str_replace_all str_split str_trim
#' @importFrom readr read_file
#' @importFrom purrr pluck map map_chr
#' @importFrom tibble tibble
#' @importFrom tidyr separate unnest
#' @importFrom magrittr %>%
#' @importFrom utils head
#' @export
read_css <- function( file, ... ){

  rules <- read_file(file, ...) %>%
    gsub( "/[*].*?[*]/", "", . ) %>%
    str_split("[}]") %>%
    pluck(1) %>%
    head(-1) %>%
    str_split("[{]")

  tibble(
      rule = str_trim(map_chr(rules, 1)),
      data = str_split(map_chr(rules, 2), ";(?!base64)") %>%
        map(str_trim) %>%
        map(not_empty)
    ) %>%
    unnest() %>%
    separate( data, into = c( "setting", "value" ), sep = ":", extra = "merge")

}

#' @importFrom utils globalVariables
globalVariables("data")
