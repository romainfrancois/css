
not_empty <- function(x){
  x[ x != ""]
}

#' @importFrom stringr str_replace_all str_split str_trim
#' @importFrom readr read_file
#' @importFrom purrr pluck map map_chr
#' @importFrom tibble tibble
#' @importFrom tidyr separate unnest
#' @importFrom magrittr %>%
#' @importFrom utils head
read_css <- function( file ){

  rules <- read_file(file) %>%
    str_replace_all("[/][*].*[*][/]", "") %>%
    str_split("[}]") %>%
    pluck(1) %>%
    head(-1) %>%
    str_split("[{]")

  tibble(
      rule = map_chr(rules, 1),
      data = str_split(map_chr(rules, 2), ";(?!base64)") %>%
        map(str_trim) %>%
        map(not_empty)
    ) %>%
    unnest() %>%
    separate( data, into = c( "setting", "value" ), sep = ":", extra = "merge")

}

#' @importFrom utils globalVariables
globalVariables("data")
