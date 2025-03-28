package org.intellij.sdk.language;

import com.intellij.lang.ASTNode;
import com.intellij.lang.folding.FoldingBuilderEx;
import com.intellij.lang.folding.FoldingDescriptor;
import com.intellij.openapi.editor.Document;
import com.intellij.openapi.project.DumbAware;
import com.intellij.openapi.util.TextRange;
import com.intellij.psi.PsiElement;
import com.intellij.psi.SyntaxTraverser;
import org.intellij.sdk.language.psi.YakshaClassBlock;
import org.intellij.sdk.language.psi.YakshaDefBlock;
import org.intellij.sdk.language.psi.YakshaDslOuterBlock;
import org.intellij.sdk.language.psi.YakshaEnumBlock;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.List;
import java.util.Optional;

public class YakshaFolderBuilder extends FoldingBuilderEx implements DumbAware {
    @Override
    public FoldingDescriptor @NotNull [] buildFoldRegions(
            @NotNull PsiElement root, @NotNull Document document, boolean quick) {
        final List<Optional<FoldingDescriptor>> items = SyntaxTraverser.psiTraverser(root)
                .filter(i ->
                        (i instanceof YakshaClassBlock ||
                                i instanceof YakshaDefBlock ||
                                i instanceof YakshaEnumBlock ||
                                i instanceof YakshaDslOuterBlock)
                )
                .map(YakshaFolderBuilder::makeFold).toList();
        return items.stream().filter(Optional::isPresent).map(Optional::get)
                .toArray(FoldingDescriptor[]::new);
    }

    @Override
    public @Nullable String getPlaceholderText(@NotNull ASTNode node) {
        return "...";
    }

    @Override
    public boolean isCollapsedByDefault(@NotNull ASTNode node) {
        return false;
    }

    private static TextRange getRange(PsiElement elm) {
        TextRange range = elm.getTextRange();
        final String text = elm.getText();
        if (elm instanceof YakshaDefBlock || elm instanceof YakshaClassBlock || elm instanceof YakshaEnumBlock || elm instanceof YakshaDslOuterBlock) {
            if (text.endsWith("\r\n\r\n")) {
                return new TextRange(range.getStartOffset(), range.getEndOffset() - 4);
            } else if (text.endsWith("\r\n")) {
                return new TextRange(range.getStartOffset(), range.getEndOffset() - 2);
            } else if (text.endsWith("\n\n")) {
                return new TextRange(range.getStartOffset(), range.getEndOffset() - 2);
            } else if (text.endsWith("\n")) {
                return new TextRange(range.getStartOffset(), range.getEndOffset() - 1);
            }
        }
        return range;
    }

    private static Optional<FoldingDescriptor> makeFold(PsiElement elm) {
        try {
            return Optional.of(new YakshaFoldingDesc(elm, getRange(elm)));
        } catch (AssertionError ignored) {
        }
        return Optional.empty();
    }

    public static class YakshaFoldingDesc extends FoldingDescriptor {

        public YakshaFoldingDesc(PsiElement element, TextRange range) {
            super(element.getNode(), range);
        }

        @Override
        public @Nullable String getPlaceholderText() {
            return "...";
        }
    }
}
