# Makefile for job_submit/gres_env plugin

AUTOMAKE_OPTIONS = foreign

PLUGIN_FLAGS = -module -avoid-version --export-dynamic

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/src/common

pkglib_LTLIBRARIES = job_submit_gres_env.la

# Job submit gres_env plugin.
job_submit_gres_env_la_SOURCES = job_submit_gres_env.c
job_submit_gres_env_la_LDFLAGS = $(PLUGIN_FLAGS)
