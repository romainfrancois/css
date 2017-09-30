
[![Travis-CI Build Status](https://travis-ci.org/romainfrancois/css.svg?branch=master)](https://travis-ci.org/romainfrancois/css)

<!-- README.md is generated from README.Rmd. Please edit that file -->
css
===

`css` eases the manipulation of css files with the functions `read_css` and `write_css`.

Installation
------------

You can install css from github with:

``` r
# install.packages("devtools")
devtools::install_github("romainfrancois/css")
```

Example
-------

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

You can then tidyverse it a bit and then save it back to a file using `write_css`:

``` r
library(dplyr)
#> Warning: package 'dplyr' was built under R version 3.4.1
#> 
#> Attaching package: 'dplyr'
#> The following objects are masked from 'package:stats':
#> 
#>     filter, lag
#> The following objects are masked from 'package:base':
#> 
#>     intersect, setdiff, setequal, union
library(readr)

data <- data %>% 
  mutate( value = case_when(
    setting == "background-color" ~ "#ff0000", 
    TRUE                          ~ value
  ))

tf <- tempfile()
write_css(data, tf)
writeLines( read_lines(tf, n_max = 10))
#> .ace_bracket { 
#>    margin:  0 !important ; 
#>    border:  0 !important ; 
#>    background-color: #ff0000 ; 
#>    margin:  0 !important ; 
#>    border:  0 !important ; 
#>    background-color: #ff0000 ;  
#>  }
#> .ace_comment { 
#>    color:  #6272a4 ;
```
