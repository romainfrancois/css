
[![Travis-CI Build Status](https://travis-ci.org/romainfrancois/css.svg?branch=master)](https://travis-ci.org/romainfrancois/css)

<!-- README.md is generated from README.Rmd. Please edit that file -->
css
===

The goal of `css` is to turn css files into tibbles

Installation
------------

You can install css from github with:

``` r
# install.packages("devtools")
devtools::install_github("romainfrancois/css")
```

Example
-------

This is a basic example which shows you how to solve a common problem:

``` r
library(css)

# one rstudio css theme
dracula <- paste0(
  "https://raw.githubusercontent.com/rstudio/rstudio/", 
  "0d9d49aa8671527370c6c261fc031ae8a8081c45/src/gwt/src/",   
  "org/rstudio/studio/client/workbench/views/source/editors/text/themes/dracula.css"
)

# reading it with read_css
data <- read_css(dracula)

# ... now it's a tibble ... 
dplyr::glimpse(data)
#> Observations: 626
#> Variables: 3
#> $ rule    <chr> ".ace_gutter", ".ace_gutter", ".ace_print-margin", ".a...
#> $ setting <chr> "background", "color", "width", "background", "backgro...
#> $ value   <chr> " #282a36", " rgb(144,145,148)", " 1px", " #e8e8e8", "...
DT::datatable(data)
```

![](README-example-1.png)
