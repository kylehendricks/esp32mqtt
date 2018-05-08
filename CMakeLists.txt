set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_PRIV_INCLUDEDIRS lib/include)

# Edit following two lines to set component requirements (see docs)
set(COMPONENT_PRIV_REQUIRES lwip mbedtls nghttp)

set(COMPONENT_SRCDIRS . lib)

register_component()
