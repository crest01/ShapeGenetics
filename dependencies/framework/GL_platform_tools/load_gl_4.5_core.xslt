<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:key name="type-lookup" match="types/type[not(@api) or @api='gl']" use="name" />
	<xsl:key name="enum-lookup" match="enums/enum" use="@name" />
	<xsl:key name="command-lookup" match="commands/command" use="proto/name" />
	<xsl:key name="core-profile-removed-enum-lookup" match="feature[@api='gl' and @number&lt;=4.5]/remove[not(@profile) or @profile='core']/enum" use="@name" />
	<xsl:key name="core-profile-removed-command-lookup" match="feature[@api='gl' and @number&lt;=4.5]/remove[not(@profile) or @profile='core']/command" use="@name" />

	<xsl:variable name="API-enums" select="registry
		/feature[@api='gl' and @number&lt;=4.5]
		/require[not(@profile) or @profile='core']
		/enum[not(key('core-profile-removed-command-lookup', @name))]" />

	<xsl:variable name="API-commands" select="registry
		/feature[@api='gl' and @number&lt;=4.5]
		/require[not(@profile) or @profile='core']
		/command[not(key('core-profile-removed-command-lookup', @name))]" />

	<xsl:variable name="enums" select="key('enum-lookup', $API-enums/@name)" />
	<xsl:variable name="commands" select="key('command-lookup', $API-commands/@name)" />

</xsl:stylesheet>
