# Makefile for cli_filter/gres_env plugin

AUTOMAKE_OPTIONS = foreign

PLUGIN_FLAGS = -module -avoid-version --export-dynamic

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/src/common

pkglib_LTLIBRARIES = cli_filter_gres_env.la spank_gres_env.la

# Job submit defaults plugin.
cli_filter_gres_env_la_SOURCES = cli_filter_gres_env.c
cli_filter_gres_env_la_LDFLAGS = $(SO_LDFLAGS) $(PLUGIN_FLAGS)
cli_filter_gres_env_la_LIBADD = ../common/libcli_filter_common.la

# SPANK plugin.
spank_gres_env_la_SOURCES = spank_gres_env.c
spank_gres_env_la_LDFLAGS = $(PLUGIN_FLAGS)
