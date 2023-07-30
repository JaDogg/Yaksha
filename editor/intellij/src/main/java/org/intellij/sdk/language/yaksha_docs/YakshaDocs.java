package org.intellij.sdk.language.yaksha_docs;

import com.google.common.collect.ImmutableMap;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;
import com.intellij.codeInsight.completion.CompletionResultSet;
import com.intellij.codeInsight.lookup.LookupElementBuilder;
import org.intellij.sdk.language.DocBuilder;
import org.intellij.sdk.language.YakshaIcons;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.lang.reflect.Type;
import java.util.*;

public class YakshaDocs {
    public static final Map<String, BuiltinDoc> BUILTIN_FUNCTIONS = ImmutableMap.<String, BuiltinDoc>builder()
            .put("print", BuiltinDoc.b("print(primitive) -> None", "Print without a new line"))
            .put("println", BuiltinDoc.b("println(primitive) -> None", "Print + new line"))
            .put("len", BuiltinDoc.b("len(Array[T]) -> int", "Get length of arrays,maps"))
            .put("arrput", BuiltinDoc.b("arrput(Array[T], T) -> None", "Put item to an array"))
            .put("arrpop", BuiltinDoc.b("arrpop(Array[T]) -> T", "Remove last item from an array and return it"))
            .put("arrnew", BuiltinDoc.b("arrnew(\"T\", int) -> Array[T]", "Create a new array of given size. " +
                    "(Uninitialized elements)"))
            .put("arrsetcap", BuiltinDoc.b("arrsetcap(Array[T], int) -> None", "Set array capacity / grow memory. " +
                    "Does not affect length."))
            .put("arrsetlen", BuiltinDoc.b("arrsetlen(Array[T], int) -> None", "Set array length. Each element will " +
                    "be an uninitialized element."))
            .put("array", BuiltinDoc.b("array(\"T\", T...) -> Array[T]", "Create a new array from given elements"))
            .put("getref", BuiltinDoc.b("getref(T) -> Ptr[T]", "Get a pointer to given object"))
            .put("unref", BuiltinDoc.b("unref(Ptr[T]) -> T", "Dereference a pointer"))
            .put("charat", BuiltinDoc.b("charat(str, int) -> int", "Get a character at a specific location in string"))
            .put("shnew", BuiltinDoc.b("shnew(Array[SMEntry[T]]) -> None", "Initialize Array[SMEntry[T]] object"))
            .put("shput", BuiltinDoc.b("shput(Array[SMEntry[T]], str, T) -> None", "Put item to a Array[SMEntry[T]]"))
            .put("shget", BuiltinDoc.b("shget(Array[SMEntry[T]], str) -> T", "Get item from a Array[SMEntry[T]]"))
            .put("shgeti", BuiltinDoc.b("shgeti(Array[SMEntry[T]], str) -> int", "Get item index from a " +
                    "Array[SMEntry[T]] (-1 if not found)"))
            .put("hmnew", BuiltinDoc.b("hmnew(Array[MEntry[K,T]]) -> None", "Initialize Array[MEntry[K,T]] object"))
            .put("hmput", BuiltinDoc.b("hmput(Array[MEntry[K,T]], K, T) -> None", "Put item to a Array[MEntry[K,T]]"))
            .put("hmget", BuiltinDoc.b("hmget(Array[MEntry[K,T]], K) -> T", "Get item from a Array[MEntry[K,T]]"))
            .put("hmgeti", BuiltinDoc.b("hmgeti(Array[MEntry[K,T]], K) -> int", "Get item index from a " +
                    "Array[MEntry[K,T]] (-1 if not found)"))
            .put("cast", BuiltinDoc.b("cast(\"T\", X) -> T", "Data type casting builtin"))
            .put("qsort", BuiltinDoc.b("qsort(Array[T],Function[In[Const[AnyPtrToConst],Const[AnyPtrToConst]]," +
                    "Out[int]]) -> bool", "Sort an array, returns True if successful"))
            .put("iif", BuiltinDoc.b("iif(bool, T, T) -> T", "Ternary functionality"))
            .put("foreach", BuiltinDoc.b("foreach(Array[T],Function[In[T,V],Out[bool]],V) -> bool", "For each element" +
                    " in array execute given function"))
            .put("countif", BuiltinDoc.b("countif(Array[T],Function[In[T,V],Out[bool]],V) -> int", "For each element " +
                    "in array count if function returns true"))
            .put("filter", BuiltinDoc.b("filter(Array[T],Function[In[T,V],Out[bool]],V) -> Array[T]", "Create a new " +
                    "array with filtered elements based on return value of given function"))
            .put("map", BuiltinDoc.b("map(Array[T],Function[In[T,V],Out[K]],V) -> Array[K]", "Create a new array with" +
                    " result of given function"))
            .put("binarydata", BuiltinDoc.b("binarydata(\"data\") -> Const[Ptr[Const[u8]]]", "Create constant binary " +
                    "data (must pass in a string literal).\nReturns Const[Ptr[Const[u8]]] that does not need to be " +
                    "deleted."))
            .build();
    public static final Set<String> BUILTIN_FUNCTION_NAMES = BUILTIN_FUNCTIONS.keySet();
    public static final Set<String> LISP_BUILTIN_NAMES = Set.of("ykt_keyword_while", "ykt_keyword_return",
            "ykt_keyword_pass", "ykt_keyword_or",
            "ykt_keyword_not", "ykt_keyword_in", "ykt_keyword_import", "ykt_keyword_if", "ykt_keyword_from",
            "ykt_keyword_else",
            "ykt_keyword_elif", "ykt_keyword_del", "ykt_keyword_def", "ykt_keyword_continue", "ykt_keyword_break",
            "ykt_keyword_assert",
            "ykt_keyword_and", "ykt_keyword_true", "ykt_not_symbol", "ykt_not_eq", "ykt_shl", "ykt_comma", "ykt_dot",
            "ykt_ellipsis",
            "ykt_arrow", "ykt_power_eq", "ykt_int_div_eq", "YK_TOKEN_INTEGER_BIN_8", "YK_TOKEN_GREAT_EQ", "!=",
            "YK_TOKEN_DOUBLE_NUMBER",
            "scope", "YK_TOKEN_DIV", "try_catch", "YK_TOKEN_GREAT", "YK_TOKEN_INTEGER_BIN_64", "YK_TOKEN_AT",
            "YK_TOKEN_ARROW", "ykt_tilde", "system_disable_gc",
            "YK_TOKEN_AND_EQ", "yk_is_expr", "yk_what", "YK_TYPE_TOKEN", "nil", "YK_TOKEN_PLUS", "yk_is_stmt",
            "ykt_dedent",
            "YK_TOKEN_INTEGER_BIN", "YK_TOKEN_KEYWORD_FALSE", "YK_PRELUDE_INCLUDED", "system_unlock_root_scope",
            "ykt_int_div",
            "YK_TOKEN_CURLY_BRACKET_CLOSE", "ykt_float", "is_module", "ghost", "ykt_great", "ykt_div",
            "YK_TOKEN_POWER_EQ", "repr",
            "YK_TOKEN_SQUARE_BRACKET_OPEN", "YK_TOKEN_UINTEGER_HEX", "this", "YK_TOKEN_INT_DIV", "map_values",
            "map_set", "YK_TOKEN_KEYWORD_DEF",
            "index", "YK_TOKEN_COLON", "YK_TOKEN_AND", "YK_TOKEN_MUL", "YK_TOKEN_ELLIPSIS", "YK_TOKEN_DOT", "setq",
            "bitwise_left_shift",
            "YK_TOKEN_COMMA", "bitwise_not", "bitwise_xor", "map_remove", "YK_TOKEN_KEYWORD_FROM", "map_get",
            "map_keys", "*", "insert",
            "ykt_integer_decimal", "YK_TOKEN_DIV_EQ", "eval", "YK_TOKEN_KEYWORD_NONE", "is_int",
            "ykt_keyword_runtimefeature", "do",
            "YK_TOKEN_UINTEGER_OCT_8", "ykt_mod_eq", "map", "access_module", "if", "define", "is_metamacro",
            "YK_TOKEN_MOD_EQ",
            "def", "println", "system_disable_print", "ykt_keyword_macros", "YK_TOKEN_PAREN_OPEN",
            "YK_TOKEN_FLOAT_NUMBER",
            "YK_TOKEN_EQ", "ykt_keyword_struct", "map_has", "YK_TOKEN_UINTEGER_HEX_16", "and", "parent",
            "YK_TOKEN_NOT_EQ",
            "<=", "YK_TOKEN_INTEGER_OCT_64", "YK_TOKEN_STRING", "metamacro", "reduce", ">", "YK_TOKEN_XOR",
            "yk_get_type",
            "YK_TOKEN_INTEGER_HEX_16", "YK_TOKEN_KEYWORD_TRY", "range", "yk_register", "ykt_curly_bracket_open",
            "ykt_plus_eq",
            "is_string", "try", "bitwise_and", "ykt_mod", "+", ">=", "YK_TOKEN_BA_DEDENT", "ykt_eq", "==",
            "YK_TOKEN_INTEGER_BIN_16",
            "<", "list", "ykt_great_eq", "YK_TOKEN_KEYWORD_FOR", "YK_TOKEN_KEYWORD_CONTINUE", "YK_TOKEN_BA_INDENT",
            "YK_TOKEN_PLUS_EQ",
            "YK_TOKEN_KEYWORD_PASS", "modulo", "head", "push", "YK_TOKEN_KEYWORD_WHILE", "noop", "YK_TOKEN_MOD", "for",
            "filter", "ykt_indent", "false", "ykt_string", "or", "ykt_double", "cons", "is_nil", "YK_TOKEN_KEYWORD_IN",
            "YK_TOKEN_EQ_EQ", "YK_TOKEN_SUB", "ykt_keyword_ccode", "YK_TOKEN_UINTEGER_BIN", "len", "ykt_keyword_defer",
            "ykt_shl_eq", "ykt_paren_close", "YK_TOKEN_UINTEGER_OCT_64", "ykt_less_eq", "=", "not", "is_truthy",
            "pop", "YK_TOKEN_CURLY_BRACKET_OPEN", "raise_error", "io_write_file", "lambda", "to_string",
            "YK_TOKEN_UINTEGER_DECIMAL_8",
            "YK_TOKEN_SHL", "/", "YK_TOKEN_KEYWORD_STRUCT", "yk_is_token", "print", "system_enable_print", "true",
            "io_read_file",
            "input", "is_list", "YK_TOKEN_UINTEGER_BIN_16", "YK_TOKEN_UINTEGER_OCT", "ykt_keyword_none", "YK_KEY_WHAT",
            "to_int", "random", "time", "YK_TOKEN_INTEGER_DECIMAL", "YK_TOKEN_INTEGER_DECIMAL_16",
            "YK_TOKEN_INTEGER_DECIMAL_8",
            "system_enable_gc", "YK_TOKEN_INTEGER_HEX", "YK_TOKEN_INTEGER_OCT_16", "YK_TOKEN_INTEGER_HEX_64", "parse",
            "YK_TOKEN_SUB_EQ", "yk_create_token", "YK_TOKEN_INTEGER_OCT", "YK_TOKEN_INTEGER_OCT_8",
            "YK_TOKEN_INT_DIV_EQ",
            "bitwise_right_shift", "YK_TOKEN_LESS", "YK_TOKEN_LESS_EQ", "YK_TOKEN_NAME", "ykt_keyword_for",
            "ykt_mul_eq",
            "YK_TOKEN_SHR_EQ", "YK_TOKEN_NOT_SYMBOL", "-", "YK_TOKEN_OR_EQ", "ykt_colon", "YK_TOKEN_POWER",
            "YK_TOKEN_SHR",
            "YK_OBJECT_TYPE", "ykt_or", "ykt_keyword_try", "YK_TOKEN_MUL_EQ", "YK_TOKEN_SHL_EQ",
            "YK_TOKEN_THREE_QUOTE_STRING",
            "YK_TOKEN_UINTEGER_BIN_64", "YK_TOKEN_UINTEGER_BIN_8", "YK_TOKEN_INTEGER_HEX_8",
            "YK_TOKEN_UINTEGER_DECIMAL",
            "YK_TOKEN_UINTEGER_DECIMAL_16", "YK_TOKEN_OR", "YK_TOKEN_UINTEGER_DECIMAL_64", "YK_TOKEN_UINTEGER_HEX_64",
            "YK_TOKEN_UINTEGER_HEX_8", "YK_TOKEN_PAREN_CLOSE", "YK_TOKEN_UINTEGER_OCT_16", "YK_TOKEN_XOR_EQ",
            "YK_TOKEN_KEYWORD_TRUE",
            "quote", "YK_TOKEN_KEYWORD_AND", "YK_TOKEN_KEYWORD_AS", "ykt_keyword_false", "YK_TOKEN_KEYWORD_BREAK",
            "YK_TOKEN_KEYWORD_CCODE",
            "while", "newline", "ykt_mul", "YK_TOKEN_KEYWORD_CLASS", "ykt_at", "ykt_keyword_class", "ykt_power",
            "YK_TOKEN_KEYWORD_DEFER",
            "ykt_shr_eq", "YK_TOKEN_KEYWORD_DEL", "YK_TOKEN_KEYWORD_ELIF", "ykt_keyword_as", "YK_TOKEN_KEYWORD_ELSE",
            "YK_TOKEN_KEYWORD_IF",
            "YK_TOKEN_KEYWORD_ASSERT", "YK_TOKEN_KEYWORD_IMPORT", "ykt_shr", "YK_TOKEN_KEYWORD_MACROS",
            "YK_TOKEN_TILDE", "YK_TOKEN_KEYWORD_NOT",
            "YK_TOKEN_KEYWORD_OR", "magic_dot", "ykt_xor", "YK_TOKEN_KEYWORD_RETURN",
            "YK_TOKEN_KEYWORD_RUNTIMEFEATURE", "tail", "is_callable",
            "ykt_newline", "defun", "ykt_paren_open", "bitwise_or", "ykt_square_bracket_open",
            "ykt_square_bracket_close", "ykt_curly_bracket_close", "ykt_less", "ykt_plus",
            "YK_TOKEN_INTEGER_DECIMAL_64", "YK_TOKEN_NEW_LINE", "ykt_sub", "ykt_and", "ykt_eq_eq", "cond", "ykt_or_eq"
            , "system_lock_root_scope", "remove", "ykt_sub_eq", "ykt_div_eq", "YK_TOKEN_SQUARE_BRACKET_CLOSE",
            "ykt_and_eq", "ykt_xor_eq");
    public static final YakshaDocs INSTANCE = new YakshaDocs();
    private final Map<String, Doc> doc;

    public YakshaDocs() {
        final Type typeOfHashmap = new TypeToken<Map<String, Doc>>() {
        }.getType();
        final Gson gson = new GsonBuilder().create();

        Map<String, Doc> doc;
        try (Reader reader = new InputStreamReader(Objects
                .requireNonNull(this.getClass().getResourceAsStream("/apidocs/docs.json")))) {
            doc = gson.fromJson(reader, typeOfHashmap);
        } catch (IOException e) {
            doc = new HashMap<>();
        }
        this.doc = doc;
    }

    public boolean fillTo(CompletionResultSet resultSet, String importPath) {
        Doc documentation = doc.get(importPath);
        if (documentation == null) {
            return false;
        }
        for (GlobalConstant c : documentation.global_consts) {
            resultSet.addElement(LookupElementBuilder.create(c.name)
                    .withIcon(YakshaIcons.CONSTANT)
                    .withTypeText(c.getTypeText())
                    .withPresentableText(c.getRepr())
            );
        }
        for (Fn f : documentation.functions) {
            resultSet.addElement(LookupElementBuilder.create(f.name)
                    .withIcon(YakshaIcons.DEF)
                    .withTypeText(f.getTypeText())
                    .withPresentableText(f.getRepr())
            );
        }

        for (Cls cl : documentation.classes) {
            resultSet.addElement(LookupElementBuilder.create(cl.name)
                    .withIcon(YakshaIcons.CLASS)
                    .withTypeText(cl.getTypeText())
                    .withPresentableText(cl.getRepr())
            );
        }
        return true;
    }

    public String generateDoc(String importPath, String name) {
        Doc documentation = doc.get(importPath);
        if (documentation == null) {
            return "";
        }
        // TODO hashmap this thing to make it faster
        for (GlobalConstant c : documentation.global_consts) {
            if (c.name.equals(name)) {
                return c.genDoc(importPath);
            }
        }
        for (Fn f : documentation.functions) {
            if (f.name.equals(name)) {
                return f.genDoc(importPath);
            }
        }

        for (Cls cl : documentation.classes) {
            if (cl.name.equals(name)) {
                return cl.genDoc(importPath);
            }
        }
        return "";
    }

    private static boolean keepOut(final String filter, final String haystack, final String haystack2) {
        if (filter == null || filter.isBlank()) {
            return false;
        }
        return !((!haystack.isBlank() && haystack.toLowerCase(Locale.ENGLISH).contains(filter))
                || (!haystack2.isBlank() && haystack2.toLowerCase(Locale.ENGLISH).contains(filter)));
    }

    public void fillTo(DefaultMutableTreeNode root, final String filter) {
        final String filterLowerCase = filter.toLowerCase(Locale.ENGLISH);
        DefaultMutableTreeNode builtins = new DefaultMutableTreeNode("builtins");
        BUILTIN_FUNCTIONS.forEach((k, v) -> {
            if (keepOut(filterLowerCase, v.typeDoc, v.comment)) {
                return;
            }
            DefaultMutableTreeNode node = new DefaultMutableTreeNode(DocWithIcon.dwi(v.typeDoc, YakshaIcons.BUILT_IN));
            addComments(v.comment, node);
            builtins.add(node);
        });
        if (builtins.getChildCount() > 0) {
            root.add(builtins);
        }
        doc.forEach((k, d) -> {
            DefaultMutableTreeNode lib = new DefaultMutableTreeNode(k);
            d.fill(lib, filterLowerCase);
            if (lib.getChildCount() > 0) {
                root.add(lib);
            }
        });
    }

    private static void addComments(String allComments, DefaultMutableTreeNode node) {
        if (allComments.isBlank()) {
            return;
        }
        String[] comments = allComments.split("\\n");
        for (final String commentLine : comments) {
            node.add(new DefaultMutableTreeNode(DocWithIcon.dwi(commentLine, YakshaIcons.COMMENT)));
        }
    }

    public static class Doc {
        public List<Import> imports;
        public List<GlobalConstant> global_consts;
        public List<Fn> functions;
        public List<Cls> classes;

        public void fill(DefaultMutableTreeNode lib, String filterLowerCase) {
            global_consts.forEach(c -> {
                if (keepOut(filterLowerCase, c.getRepr(), c.getTypeText())) {
                    return;
                }
                DefaultMutableTreeNode node = new DefaultMutableTreeNode(DocWithIcon.dwi(c.getRepr(),
                        YakshaIcons.CONSTANT));
                addComments(c.getTypeText(), node);
                lib.add(node);
            });
            classes.forEach(c -> {
                if (keepOut(filterLowerCase, c.name, c.getTypeText())) {
                    return;
                }
                DefaultMutableTreeNode node = new DefaultMutableTreeNode(DocWithIcon.dwi(c.name, YakshaIcons.CLASS));
                addComments(c.getTypeText(), node);
                for (Param p : c.members) {
                    node.add(new DefaultMutableTreeNode(DocWithIcon.dwi(p.toString(), YakshaIcons.SUB_SECTION)));
                }
                lib.add(node);
            });
            functions.forEach(c -> {
                if (keepOut(filterLowerCase, c.getRepr(), c.getTypeText())) {
                    return;
                }
                DefaultMutableTreeNode node = new DefaultMutableTreeNode(DocWithIcon.dwi(c.getRepr(), YakshaIcons.DEF));
                addComments(c.getTypeText(), node);
                lib.add(node);
            });
        }
    }

    public static class Import {
        public String alias;
        public List<String> path;
    }

    public static class GlobalConstant {
        public String name;
        public String comment;
        public Dt datatype;

        public String getRepr() {
            return name + ": " + datatype.toString();
        }

        public String getTypeText() {
            return comment;
        }

        public String genDoc(final String importPath) {
            DocBuilder b = new DocBuilder();
            b.title(importPath + "." + name);
            b.description(comment.replace("\n", "<br />"));
            b.keyValue("<b>Kind</b>", "Constant");
            b.keyValue("Data Type", datatype.toString());
            return b.build();
        }
    }

    public static class Fn {
        public String name;
        public String comment;
        public Dt return_type;
        public List<Param> parameters;
        public List<Ant> annotations;

        public String getRepr() {
            StringBuilder b = new StringBuilder();
            b.append(name).append("(");
            if (parameters != null && !parameters.isEmpty()) {
                final StringJoiner j = new StringJoiner(", ");
                parameters.forEach(e -> j.add(e.toString()));
                b.append(j);
            }
            b.append(") -> ").append(return_type.toString());
            return b.toString();
        }

        public String getTypeText() {
            return comment;
        }

        public String genDoc(final String importPath) {
            DocBuilder b = new DocBuilder();
            b.title(importPath + "." + name);
            b.description(comment.replace("\n", "<br />"));
            b.keyValue("<b>Kind</b>", "Function");
            if (parameters != null && !parameters.isEmpty()) {
                parameters.forEach(p -> b.typeKeyValue("Param", p.name, p.datatype.toString()));
            }
            b.keyValue("Return Type", return_type.toString());
            return b.build();
        }
    }

    public static class Dt {
        public String type;
        public String module;
        public List<Dt> arguments;

        @Override
        public String toString() {
            StringBuilder b = new StringBuilder();
            if (module != null && !module.isEmpty()) {
                b.append(module).append(".");
            }
            b.append(type);
            if (arguments != null && !arguments.isEmpty()) {
                b.append("[");
                final StringJoiner j = new StringJoiner(", ");
                arguments.forEach(e -> j.add(e.toString()));
                b.append(j);
                b.append("]");
            }
            return b.toString();
        }
    }

    public static class Ant {
        public String name;
        public String argument;
    }

    public static class Param {
        public String name;
        public Dt datatype;

        @Override
        public String toString() {
            return name + ": " + datatype;
        }
    }

    public static class Cls {
        public String name;
        public String comment;
        public List<Param> members;
        public List<Ant> annotations;

        public String getRepr() {
            StringBuilder b = new StringBuilder();
            b.append(name);
            if (members != null && !members.isEmpty()) {
                b.append(" { ");
                final StringJoiner j = new StringJoiner(", ");
                members.forEach(e -> j.add(e.toString()));
                b.append(j);
                b.append(" } ");
            }
            return b.toString();
        }

        public String getTypeText() {
            return comment;
        }

        public String genDoc(final String importPath) {
            DocBuilder b = new DocBuilder();
            b.title(importPath + "." + name);
            b.description(comment.replace("\n", "<br />"));
            b.keyValue("<b>Kind</b>", "Class");
            if (members != null && !members.isEmpty()) {
                members.forEach(p -> b.typeKeyValue("Member", p.name, p.datatype.toString()));
            }
            return b.build();
        }
    }

    public static class BuiltinDoc {
        public String typeDoc;
        public String comment;

        public static BuiltinDoc b(final String typeDoc, final String comment) {
            BuiltinDoc bd = new BuiltinDoc();
            bd.typeDoc = typeDoc;
            bd.comment = comment;
            return bd;
        }

        public String typeText() {
            return typeDoc;
        }
    }

    public static class DocWithIcon {
        public final String item;
        public final Icon icon;

        private DocWithIcon(String item, Icon icon) {
            this.item = item;
            this.icon = icon;
        }

        public static DocWithIcon dwi(String item, Icon icon) {
            return new DocWithIcon(item, icon);
        }

        @Override
        public String toString() {
            return item;
        }
    }

}
