<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:include href="helpers.xsl"/>

    <xsl:output method="text"/>

    <xsl:variable name="functionPrefix" select="registry/functionPrefix"/>

    <xsl:template match="/">
        <xsl:call-template name="add-license"/>
        <xsl:apply-templates select="registry/includes/prevalidation/include"/>
        <xsl:text>&#xA;</xsl:text>

        <xsl:call-template name="add-function-table">
            <xsl:with-param name="prefix" select="'_preval_'"/>
        </xsl:call-template>
        <xsl:apply-templates select="registry/functions/function" mode="validation"/>

        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>FunctionTable _preval_</xsl:text>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>getFunctionTable() {&#xA;    </xsl:text>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>FunctionTable functions;&#xA;&#xA;</xsl:text>
        <xsl:for-each select="registry/functions/function">
            <xsl:if test="not(@type = 'generated')">
                <xsl:text>    functions.</xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text> = _preval_</xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text>;&#xA;</xsl:text>
            </xsl:if>
        </xsl:for-each>
        <xsl:text>&#xA;    return functions;&#xA;}&#xA;&#xA;</xsl:text>
    </xsl:template>

    <xsl:template match="registry/includes/prevalidation/include">
        <xsl:text>#include &lt;</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>&gt;&#xA;</xsl:text>
    </xsl:template>

    <xsl:template match="function" mode="validation">
        <xsl:if test="not(@type = 'generated')">
            <xsl:text>static </xsl:text>
            <xsl:value-of select="return"/>
            <xsl:text> _preval_</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>(</xsl:text>
            <xsl:for-each select="param">
                <xsl:value-of select="type"/>
                <xsl:text> </xsl:text>
                <xsl:value-of select="name"/>
				<xsl:for-each select="size">
                    <xsl:text>[</xsl:text>
                    <xsl:value-of select="."/>
                    <xsl:text>]</xsl:text>
				</xsl:for-each>
                <xsl:if test="not(position() = last())">
                    <xsl:text>, </xsl:text>
                </xsl:if>
            </xsl:for-each>

            <xsl:text>) {&#xA;</xsl:text>

            <xsl:variable name="returnValue">
                <xsl:choose>
                    <xsl:when test="return = 'bs_Result'">
                        <xsl:text>BS_RESULT_VALIDATION_ERROR</xsl:text>
                    </xsl:when>
                    <xsl:when test="return = 'void'">
                    </xsl:when>
                    <xsl:when test="contains(return, '*')">
                        <xsl:text>NULL</xsl:text>
                    </xsl:when>
                    <xsl:when test="return = 'bool'">
                        <xsl:text>false</xsl:text>
                    </xsl:when>
                    <xsl:when test="/registry/structures/structure[@name = current()/return]">
                        <xsl:text>(</xsl:text>
                        <xsl:value-of select="return"/>
                        <xsl:text>) { 0 }</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>0</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:variable>

            <xsl:for-each select="param">
                <xsl:if test="contains(type, '*')">
                    <xsl:text>    </xsl:text>
                    <xsl:value-of select="/registry/functionPrefixCaps"/>
                    <xsl:text>VALIDATE(</xsl:text>
                    <xsl:value-of select="name"/>
                    <xsl:text> != NULL, </xsl:text>
                    <xsl:value-of select="$returnValue"/>
                    <xsl:text>,);&#xA;</xsl:text>

                    <!-- Object type safety check-->
                    <xsl:variable name="objectType" select="/registry/structures/structure[@name = translate(current()/type, '*', '')]/@object"/>
                    <xsl:if test="not(contains(type, '**')) and $objectType and not(size)">
                        <xsl:text>    </xsl:text>
                        <xsl:value-of select="/registry/functionPrefixCaps"/>
                        <xsl:text>VALIDATE(</xsl:text>
                        <xsl:value-of select="name"/>
                        <xsl:text>->head.source_id != </xsl:text>
                        <xsl:value-of select="$objectType"/>
                        <xsl:text>, </xsl:text>
                        <xsl:value-of select="$returnValue"/>
                        <xsl:text>,);&#xA;</xsl:text>
                    </xsl:if>
                </xsl:if>
            </xsl:for-each>

			<xsl:text>    </xsl:text>
			<xsl:if test="not(return = 'void')">
				<xsl:text>return </xsl:text>
			</xsl:if>
			<xsl:text>next.</xsl:text>
			<xsl:value-of select="@name"/>
            <xsl:text>(</xsl:text>
            <xsl:for-each select="param">
                <xsl:value-of select="name"/>
                <xsl:if test="not(position() = last())">
                    <xsl:text>, </xsl:text>
                </xsl:if>
            </xsl:for-each>           
            <xsl:text>);&#xA;}&#xA;</xsl:text>

            <xsl:text>&#xA;</xsl:text>
        </xsl:if>
    </xsl:template>

    <xsl:template match="function" mode="enableValidation">
        <xsl:if test="not(@type = 'generated')">
            <xsl:text>    </xsl:text>
            <xsl:text>functions.</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text> = _preval_</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>;&#xA;</xsl:text>
        </xsl:if>
    </xsl:template>

    <xsl:template match="function" mode="disableValidation">
        <xsl:if test="not(@type = 'generated')">
            <xsl:text>    </xsl:text>
            <xsl:text>functions.</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text> = _</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>;&#xA;</xsl:text>
        </xsl:if>
    </xsl:template>

</xsl:stylesheet>
