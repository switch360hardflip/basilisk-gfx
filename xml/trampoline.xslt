<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:include href="helpers.xsl"/>

    <xsl:output method="text"/>
    
    <xsl:strip-space elements="*"/>

    <xsl:variable name="prefix" select="registry/prefix"/>
    <xsl:variable name="functionPrefix" select="registry/functionPrefix"/>
    <xsl:variable name="file" select="registry/file"/>

    <xsl:template match="/">
        <xsl:call-template name="add-license"/>
        <xsl:apply-templates select="registry/includes/trampoline/include"/>
        <xsl:text>&#xA;</xsl:text>
		
        <xsl:value-of select="$functionPrefix"/>
        <xsl:text>FunctionTable next = { 0 };&#xA;</xsl:text>
        <xsl:text>&#xA;</xsl:text>
		
        <xsl:apply-templates select="registry/functions/*" mode="definition"/>
    </xsl:template>

    <xsl:template match="registry/includes/trampoline/include">
        <xsl:text>#include &lt;</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>&gt;&#xA;</xsl:text>
    </xsl:template>

    <xsl:template match="function" mode="definition">
        <xsl:value-of select="return"/>
        <xsl:text> </xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>(</xsl:text>

        <xsl:for-each select="param">
            <xsl:text>&#xA;    </xsl:text>
            <xsl:if test="type">
                <xsl:value-of select="type"/>
                <xsl:text> </xsl:text>
            </xsl:if>

            <xsl:value-of select="name"/>
            <xsl:for-each select="size">
                <xsl:text>[</xsl:text>
                <xsl:value-of select="."/>
                <xsl:text>]</xsl:text>
            </xsl:for-each>
            <xsl:if test="position() != last()">
                <xsl:text>, </xsl:text>
            </xsl:if>
        </xsl:for-each>
        <xsl:text>)&#xA;{&#xA;</xsl:text>

        <!-- Body -->
        <xsl:value-of select="body"/>

        <xsl:if test="not(body)">
            <xsl:text>    return next.</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>(</xsl:text>

            <xsl:for-each select="param">
                <xsl:value-of select="name"/>
                <xsl:if test="position() != last()">
                    <xsl:text>, </xsl:text>
                </xsl:if>
            </xsl:for-each>
            <xsl:text>);</xsl:text>
        </xsl:if>

        <xsl:text>&#xA;}&#xA;&#xA;</xsl:text>
    </xsl:template>

</xsl:stylesheet>
