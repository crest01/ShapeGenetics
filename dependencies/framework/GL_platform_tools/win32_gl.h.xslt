<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:import href="enum.xslt" />
	<xsl:import href="command.xslt" />
	<xsl:import href="win32_load_gl_4.5_core.xslt" />

	<xsl:output method="text" encoding="utf-8" />
	<xsl:strip-space elements="*" />


	<xsl:template match="apientry">
		<xsl:text>APIENTRY</xsl:text>
	</xsl:template>
	
	<xsl:template match="/">
		<xsl:text>


#ifndef INCLUDED_GL_API
#define INCLUDED_GL_API

#pragma once

#include &lt;cstddef&gt;
#include &lt;cstdint&gt;

#include &lt;win32/platform.h&gt;
#include &lt;win32/glcore.h&gt;


#ifdef __cplusplus
extern "C"
{
#endif
</xsl:text>
		<xsl:for-each select="key('type-lookup', $commands//ptype)">
			<xsl:apply-templates />
			<xsl:text>
</xsl:text>
		</xsl:for-each>

		<xsl:text>
</xsl:text>

		<xsl:for-each select="$enums">
			<xsl:call-template name="enum-write-declaration" />
		</xsl:for-each>

		<xsl:text>
</xsl:text>

		<xsl:for-each select="$link-commands">
			<xsl:text>WINGDIAPI </xsl:text>
			<xsl:call-template name="command-write-function-declaration">
				<xsl:with-param name="calling-convention" select="'APIENTRY'" />
			</xsl:call-template>
		</xsl:for-each>

		<xsl:text>
</xsl:text>

		<xsl:for-each select="$load-commands">
			<xsl:text>GLCOREAPI </xsl:text>
			<xsl:call-template name="command-write-function-declaration">
				<xsl:with-param name="calling-convention" select="'APIENTRY'" />
			</xsl:call-template>
		</xsl:for-each>

		<xsl:text>
GLCOREAPI BOOL APIENTRY wglSwapIntervalEXT(int interval);
#ifdef __cplusplus
}
#endif

#endif  // INCLUDED_GL_API
</xsl:text>
	</xsl:template>
</xsl:stylesheet>
