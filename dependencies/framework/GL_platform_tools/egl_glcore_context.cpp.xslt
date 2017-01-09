<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:import href="enum.xslt" />
	<xsl:import href="command.xslt" />
	<xsl:import href="load_gl_4.5_core.xslt" />

	<xsl:output method="text" encoding="utf-8" />
	<xsl:strip-space elements="*" />


	<xsl:template match="/">
		<xsl:text>


#include &lt;stdexcept&gt;

#include "context.h"


namespace
{
	thread_local const glcoreContext* context = nullptr;
}

extern "C"
{
	glcoreContext::glcoreContext()
	{
</xsl:text>
		<xsl:for-each select="$API-commands">
			<xsl:text>		</xsl:text>
			<xsl:value-of select="substring(@name, 3)" />
			<xsl:text> = reinterpret_cast&lt;decltype(</xsl:text>
			<xsl:value-of select="@name" />
			<xsl:text>)*&gt;(eglGetProcAddress("</xsl:text>
			<xsl:value-of select="@name" />
			<xsl:text>"));
</xsl:text>
		</xsl:for-each>
		<xsl:text>
		if (</xsl:text>
		<xsl:value-of select="substring($API-commands[1]/@name, 3)"/>
		<xsl:text> == nullptr</xsl:text>
		<xsl:for-each select="$API-commands[position() &gt; 1]">
				<xsl:text> ||
		    </xsl:text>
				<xsl:value-of select="substring(@name, 3)" />
				<xsl:text> == nullptr</xsl:text>
		</xsl:for-each>
		<xsl:text>)
			return; // throw std::runtime_error("OpenGL IAT initialization failed");
	}

	const glcoreContext* glcoreContextInit()
	{
		return new glcoreContext;
	}
	
	void glcoreContextDestroy(const glcoreContext* ctx)
	{
		delete ctx;
	}
	
	void glcoreContextMakeCurrent(const glcoreContext* ctx)
	{
		context = ctx;
	}
	
	const glcoreContext* glcoreContextGetCurrent()
	{
		return context;
	}

</xsl:text>
		<xsl:for-each select="$commands">
			<xsl:text>
	</xsl:text>
			<xsl:call-template name="command-write-function-return-type" />
			<xsl:text> </xsl:text>
			<xsl:call-template name="command-write-function-signature" />
			<xsl:text>
	{
		return context-></xsl:text>
			<xsl:call-template name="command-write-function-call">
				<xsl:with-param name="identifier" select="substring(proto/name, 3)" />
			</xsl:call-template>
			<xsl:text>;
	}
</xsl:text>
		</xsl:for-each>
<xsl:text>}
</xsl:text>
	</xsl:template>
</xsl:stylesheet>
