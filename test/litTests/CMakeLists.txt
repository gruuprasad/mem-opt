set(TAS_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
configure_file(lit.site.cfg.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
  @ONLY
)
