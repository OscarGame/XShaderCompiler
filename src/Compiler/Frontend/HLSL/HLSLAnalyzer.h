/*
 * HLSLAnalyzer.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2017 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_HLSL_ANALYZER_H
#define XSC_HLSL_ANALYZER_H


#include "Analyzer.h"
#include "ShaderVersion.h"
#include "Variant.h"
#include <map>


namespace Xsc
{


struct HLSLIntrinsicEntry;

// HLSL context analyzer.
class HLSLAnalyzer : public Analyzer
{
    
    public:
        
        HLSLAnalyzer(Log* log = nullptr);

    private:
        
        using OnOverrideProc = ASTSymbolTable::OnOverrideProc;
        using OnValidAttributeValueProc = std::function<bool(const AttributeValue)>;

        /* === Functions === */

        void DecorateASTPrimary(
            Program& program,
            const ShaderInput& inputDesc,
            const ShaderOutput& outputDesc
        ) override;

        void ErrorIfAttributeNotFound(bool found, const std::string& attribDesc);
        
        /* === Visitor implementation === */

        DECL_VISIT_PROC( Program           );
        DECL_VISIT_PROC( CodeBlock         );
        #if 0//TODO: remove
        DECL_VISIT_PROC( FunctionCall      );
        #endif
        DECL_VISIT_PROC( ArrayDimension    );
        DECL_VISIT_PROC( TypeSpecifier     );
        
        DECL_VISIT_PROC( VarDecl           );
        DECL_VISIT_PROC( BufferDecl        );
        DECL_VISIT_PROC( SamplerDecl       );
        DECL_VISIT_PROC( StructDecl        );
        DECL_VISIT_PROC( AliasDecl         );

        DECL_VISIT_PROC( FunctionDecl      );
        DECL_VISIT_PROC( BufferDeclStmnt   );
        DECL_VISIT_PROC( UniformBufferDecl );
        DECL_VISIT_PROC( VarDeclStmnt      );

        DECL_VISIT_PROC( CodeBlockStmnt    );
        DECL_VISIT_PROC( ForLoopStmnt      );
        DECL_VISIT_PROC( WhileLoopStmnt    );
        DECL_VISIT_PROC( DoWhileLoopStmnt  );
        DECL_VISIT_PROC( IfStmnt           );
        DECL_VISIT_PROC( ElseStmnt         );
        DECL_VISIT_PROC( SwitchStmnt       );
        DECL_VISIT_PROC( ExprStmnt         );
        DECL_VISIT_PROC( ReturnStmnt       );

        DECL_VISIT_PROC( UnaryExpr         );
        DECL_VISIT_PROC( PostUnaryExpr     );
        DECL_VISIT_PROC( FunctionCallExpr  );
        #if 0//TODO: remove
        DECL_VISIT_PROC( SuffixExpr        );
        DECL_VISIT_PROC( VarAccessExpr     );
        #endif
        #if 1//TODO: make this standard
        DECL_VISIT_PROC( AssignExpr        );
        DECL_VISIT_PROC( ObjectExpr        );
        DECL_VISIT_PROC( ArrayAccessExpr   );
        #endif

        /* ----- Function call ----- */

        void AnalyzeFunctionCallExpr(FunctionCallExpr* expr);

        void AnalyzeFunctionCall(FunctionCall* funcCall, bool isStatic = false, const Expr* prefixExpr = nullptr, const TypeDenoter* prefixTypeDenoter = nullptr);
        void AnalyzeFunctionCallStandard(FunctionCall* funcCall, bool isStatic = false, const Expr* prefixExpr = nullptr, const TypeDenoter* prefixTypeDenoter = nullptr);
        void AnalyzeFunctionCallIntrinsic(FunctionCall* funcCall, const HLSLIntrinsicEntry& intr, bool isStatic = false, const TypeDenoter* prefixTypeDenoter = nullptr);
        void AnalyzeFunctionCallIntrinsicPrimary(FunctionCall* funcCall, const HLSLIntrinsicEntry& intr);
        void AnalyzeFunctionCallIntrinsicFromBufferType(const FunctionCall* funcCall, const BufferType bufferType);

        void AnalyzeIntrinsicWrapperInlining(FunctionCall* funcCall);

        #if 0//TODO: remove

        void AnalyzeFunctionCallStandard_OBSOLETE(FunctionCall* ast);

        bool AnalyzeMemberIntrinsic(const Intrinsic intrinsic, const FunctionCall* funcCall);
        bool AnalyzeMemberIntrinsicBuffer(const Intrinsic intrinsic, const FunctionCall* funcCall, const BufferType bufferType);

        #endif

        #if 0//TODO: replace this by "ObjectExpr" and "FunctionCallExpr"

        /* ----- Variable identifier ----- */

        void AnalyzeVarIdent(VarIdent* varIdent);
        void AnalyzeVarIdentWithSymbol(VarIdent* varIdent, AST* symbol);
        void AnalyzeVarIdentWithSymbolVarDecl(VarIdent* varIdent, VarDecl* varDecl);

        void AnalyzeFunctionVarIdent(VarIdent* varIdent, const std::vector<ExprPtr>& args);
        void AnalyzeFunctionVarIdentWithSymbol(VarIdent* varIdent, const std::vector<ExprPtr>& args, AST* symbol);
        void AnalyzeFunctionVarIdentWithSymbolVarDecl(VarIdent* varIdent, const std::vector<ExprPtr>& args, VarDecl* varDecl);

        void AnalyzeVarIdentArrayIndices(VarIdent* varIdent);

        void AnalyzeLValueVarIdent(VarIdent* varIdent, const AST* ast = nullptr);
        //void AnalyzeLValueExpr(Expr* expr, const AST* ast = nullptr);

        #endif

        #if 1//TODO: make this standard

        /* ----- Object expressions ----- */

        void AnalyzeObjectExpr(ObjectExpr* expr);
        void AnalyzeObjectExprWithStruct(ObjectExpr* expr, const StructTypeDenoter& structTypeDen);

        bool AnalyzeStaticAccessExpr(const Expr* prefixExpr, bool isStatic, const AST* ast = nullptr);
        bool AnalyzeStaticTypeSpecifier(const TypeSpecifier* typeSpecifier, const std::string& ident, const Expr* expr, bool isStatic);

        void AnalyzeLValueExpr(const Expr* expr, const AST* ast = nullptr);

        /* ----- Array access expression ----- */

        void AnalyzeArrayAccessExpr(ArrayAccessExpr* expr);

        #endif

        /* ----- Entry point ----- */

        void AnalyzeEntryPoint(FunctionDecl* funcDecl);
        void AnalyzeEntryPointInputOutput(FunctionDecl* funcDecl);
        //void AnalyzeEntryPointInputOutputGeometryShader(FunctionDecl* funcDecl);
        
        void AnalyzeEntryPointParameter(FunctionDecl* funcDecl, VarDeclStmnt* param);
        void AnalyzeEntryPointParameterInOut(FunctionDecl* funcDecl, VarDecl* varDecl, bool input, TypeDenoterPtr varTypeDen = nullptr);
        void AnalyzeEntryPointParameterInOutVariable(FunctionDecl* funcDecl, VarDecl* varDecl, bool input);
        void AnalyzeEntryPointParameterInOutStruct(FunctionDecl* funcDecl, StructDecl* structDecl, const std::string& structAliasName, bool input);
        void AnalyzeEntryPointParameterInOutBuffer(FunctionDecl* funcDecl, VarDecl* varDecl, BufferTypeDenoter* bufferTypeDen, bool input);
        
        void AnalyzeEntryPointAttributes(const std::vector<AttributePtr>& attribs);
        void AnalyzeEntryPointAttributesTessControlShader(const std::vector<AttributePtr>& attribs);
        void AnalyzeEntryPointAttributesTessEvaluationShader(const std::vector<AttributePtr>& attribs);
        void AnalyzeEntryPointAttributesGeometryShader(const std::vector<AttributePtr>& attribs);
        void AnalyzeEntryPointAttributesFragmentShader(const std::vector<AttributePtr>& attribs);
        void AnalyzeEntryPointAttributesComputeShader(const std::vector<AttributePtr>& attribs);

        void AnalyzeEntryPointSemantics(FunctionDecl* funcDecl, const std::vector<Semantic>& inSemantics, const std::vector<Semantic>& outSemantics);

        #if 0//TODO: replace by "AnalyzeEntryPointOutput(ObjectExpr*)" function
        void AnalyzeEntryPointOutput(VarIdent* varIdent);
        #endif

        #if 1//TODO: make this standard
        void AnalyzeEntryPointOutput(const ObjectExpr* objectExpr);
        #endif

        /* ----- Secondary entry point ----- */

        void AnalyzeSecondaryEntryPoint(FunctionDecl* funcDecl, bool isPatchConstantFunc = false);
        void AnalyzeSecondaryEntryPointAttributes(const std::vector<AttributePtr>& attribs);
        void AnalyzeSecondaryEntryPointAttributesTessEvaluationShader(const std::vector<AttributePtr>& attribs);

        /* ----- Attributes ----- */

        bool AnalyzeNumArgsAttribute(Attribute* ast, std::size_t expectedNumArgs, bool required = true);
        
        void AnalyzeAttributeDomain(Attribute* ast, bool required = true);
        void AnalyzeAttributeOutputTopology(Attribute* ast, bool required = true);
        void AnalyzeAttributePartitioning(Attribute* ast, bool required = true);
        void AnalyzeAttributeOutputControlPoints(Attribute* ast);
        void AnalyzeAttributePatchConstantFunc(Attribute* ast);

        void AnalyzeAttributeMaxVertexCount(Attribute* ast);

        void AnalyzeAttributeNumThreads(Attribute* ast);
        void AnalyzeAttributeNumThreadsArgument(Expr* ast, unsigned int& value);

        void AnalyzeAttributeValue(
            Expr* argExpr,
            AttributeValue& value,
            const OnValidAttributeValueProc& expectedValueFunc,
            const std::string& expectationDesc,
            bool required = true
        );

        bool AnalyzeAttributeValuePrimary(
            Expr* argExpr,
            AttributeValue& value,
            const OnValidAttributeValueProc& expectedValueFunc,
            std::string& literalValue
        );

        /* ----- Misc ----- */

        void AnalyzeSemantic(IndexedSemantic& semantic);

        void AnalyzeArrayDimensionList(const std::vector<ArrayDimensionPtr>& arrayDims);

        void AnalyzeParameter(VarDeclStmnt* param);
        
        /* === Members === */

        Program*            program_                    = nullptr;

        std::string         entryPoint_;
        std::string         secondaryEntryPoint_;
        bool                secondaryEntryPointFound_   = false;

        ShaderTarget        shaderTarget_               = ShaderTarget::VertexShader;
        InputShaderVersion  versionIn_                  = InputShaderVersion::HLSL5;
        ShaderVersion       shaderModel_                = { 5, 0 };
        bool                preferWrappers_             = false;

};


} // /namespace Xsc


#endif



// ================================================================================