<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:import href="load_gl_4.5_core.xslt" />

	<xsl:variable name="link-API-commands" select="$API-commands[../parent::feature/@number&lt;=1.1]" />
	<xsl:variable name="load-API-commands" select="$API-commands[../parent::feature/@number&gt;1.1]" />

	<xsl:variable name="link-commands" select="key('command-lookup', $link-API-commands/@name)" />
	<xsl:variable name="load-commands" select="key('command-lookup', $load-API-commands/@name)" />

</xsl:stylesheet>
