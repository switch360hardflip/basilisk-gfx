<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:template name="add-function-table">
        <xsl:param name="prefix"/>
        <xsl:text>static </xsl:text>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>FunctionTable next = { 0 };&#xA;&#xA;</xsl:text>
        <xsl:text>void </xsl:text>
        <xsl:value-of select="$prefix"/>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>setFunctions(</xsl:text>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>FunctionTable functions) {&#xA;    next = functions;&#xA;</xsl:text>
        <xsl:text>}&#xA;&#xA;</xsl:text>
    </xsl:template>

    <xsl:template match="ifdef|ifndef|elifdef">
        <xsl:text>#</xsl:text>
        <xsl:value-of select="name()"/>
        <xsl:text> </xsl:text>
        <xsl:value-of select="@cond"/>
        <xsl:text>&#xA;</xsl:text>
        <xsl:apply-templates select="*"/>
    </xsl:template>

    <xsl:template match="else">
        <xsl:text>#</xsl:text>
        <xsl:value-of select="name()"/>
        <xsl:text>&#xA;</xsl:text>
        <xsl:apply-templates select="*"/>
    </xsl:template>

    <xsl:template match="endif">
        <xsl:text>#endif&#xA;&#xA;</xsl:text>
    </xsl:template>

    <xsl:template name="add-function-table-getter">
        <xsl:param name="prefix"/>

        <xsl:text>static inline </xsl:text>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>FunctionTable </xsl:text>
        <xsl:value-of select="$prefix"/>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>getFunctions() {&#xA;    </xsl:text>
        <xsl:value-of select="registry/functionPrefix"/>
        <xsl:text>FunctionTable functions;&#xA;&#xA;</xsl:text>

        <xsl:text>    HMODULE module = NULL;&#xA;</xsl:text>
        <xsl:text>    GetModuleHandleExA(&#xA;        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,&#xA;        (LPCSTR)(&amp;</xsl:text>
        <xsl:value-of select="concat(concat($prefix, registry/functionPrefix), 'getFunctions')"/>
        <xsl:text>),&#xA;        &amp;module);&#xA;&#xA;</xsl:text>

        <xsl:for-each select="registry/functions/function">
            <xsl:if test="not(@type = 'generated')">
                <xsl:text>    functions.</xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text> = (PFN_</xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text>)GetProcAddress(module, "</xsl:text>
                <xsl:value-of select="$prefix"/>
                <xsl:value-of select="@name"/>
                <xsl:text>");&#xA;</xsl:text>
            </xsl:if>
        </xsl:for-each>

        <xsl:text>&#xA;    return functions;&#xA;</xsl:text>

        <xsl:text>}&#xA;&#xA;</xsl:text>
    </xsl:template>

    <xsl:template name="add-license">
        <xsl:text>
 /**
  MIT License
  
  Copyright (c) 2026 switch360hardflip &lt;switch360hardflip@gmail.com&gt;
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  */ 

 /**
  This file was generated from basilisk-gfx.com

  It is not recommended to make changes to this file as it will be lost if
  the code is regenerated.
  */
        
</xsl:text>
    </xsl:template>

</xsl:stylesheet>
