<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:import href="enum.xslt" />
	<xsl:import href="command.xslt" />
	<xsl:import href="load_gl_4.5_core.xslt" />

	<xsl:output method="text" encoding="utf-8" />
	<xsl:strip-space elements="*" />


	<xsl:template match="/">
		<xsl:text>


#ifndef INCLUDED_EGL_GLCORE_CONTEXT
#define INCLUDED_EGL_GLCORE_CONTEXT

#pragma once

#include &lt;GL/gl.h&gt;


extern "C"
{
	struct glcoreContext
	{
</xsl:text>

		<xsl:for-each select="$API-commands">
			<xsl:text>		decltype(</xsl:text>
			<xsl:value-of select="@name" />
			<xsl:text>)* </xsl:text>
			<xsl:value-of select="substring(@name, 3)" />
			<xsl:text>;
</xsl:text>
		</xsl:for-each>

<xsl:text>
		glcoreContext();
	};
}

#endif  // INCLUDED_EGL_GLCORE_CONTEXT
</xsl:text>
	</xsl:template>
</xsl:stylesheet>
