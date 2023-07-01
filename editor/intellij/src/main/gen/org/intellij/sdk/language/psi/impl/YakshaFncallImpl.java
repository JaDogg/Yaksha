// This is a generated file. Not intended for manual editing.
package org.intellij.sdk.language.psi.impl;

import java.util.List;
import org.jetbrains.annotations.*;
import com.intellij.lang.ASTNode;
import com.intellij.psi.PsiElement;
import com.intellij.psi.PsiElementVisitor;
import com.intellij.psi.util.PsiTreeUtil;
import static org.intellij.sdk.language.psi.YakshaTypes.*;
import com.intellij.extapi.psi.ASTWrapperPsiElement;
import org.intellij.sdk.language.psi.*;
import com.intellij.psi.PsiReference;

public class YakshaFncallImpl extends ASTWrapperPsiElement implements YakshaFncall {

  public YakshaFncallImpl(@NotNull ASTNode node) {
    super(node);
  }

  public void accept(@NotNull YakshaVisitor visitor) {
    visitor.visitFncall(this);
  }

  @Override
  public void accept(@NotNull PsiElementVisitor visitor) {
    if (visitor instanceof YakshaVisitor) accept((YakshaVisitor)visitor);
    else super.accept(visitor);
  }

  @Override
  @NotNull
  public List<YakshaArguments> getArgumentsList() {
    return PsiTreeUtil.getChildrenOfTypeAsList(this, YakshaArguments.class);
  }

  @Override
  @NotNull
  public List<YakshaExp> getExpList() {
    return PsiTreeUtil.getChildrenOfTypeAsList(this, YakshaExp.class);
  }

  @Override
  @NotNull
  public List<YakshaStructArguments> getStructArgumentsList() {
    return PsiTreeUtil.getChildrenOfTypeAsList(this, YakshaStructArguments.class);
  }

  @Override
  public String getDefOrClassName() {
    return YakshaPsiImplUtil.getDefOrClassName(this);
  }

  @Override
  public String getFullName() {
    return YakshaPsiImplUtil.getFullName(this);
  }

  @Override
  public PsiReference getReference() {
    return YakshaPsiImplUtil.getReference(this);
  }

}
