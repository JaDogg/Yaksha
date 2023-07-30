# YakshaIntelliJ
<!--
![Build](https://github.com/YakshaLang/YakshaIntelliJ/workflows/Build/badge.svg)
[![Version](https://img.shields.io/jetbrains/plugin/v/PLUGIN_ID.svg)](https://plugins.jetbrains.com/plugin/PLUGIN_ID)
[![Downloads](https://img.shields.io/jetbrains/plugin/d/PLUGIN_ID.svg)](https://plugins.jetbrains.com/plugin/PLUGIN_ID)

## Template ToDo list
- [x] Create a new [IntelliJ Platform Plugin Template][template] project.
- [ ] Get familiar with the [template documentation][template].
- [x] Verify the [pluginGroup](./gradle.properties), [plugin ID](./src/main/resources/META-INF/plugin.xml) and [sources package](./src/main/kotlin).
- [x] Review the [Legal Agreements](https://plugins.jetbrains.com/docs/marketplace/legal-agreements.html).
- [ ] [Publish a plugin manually](https://plugins.jetbrains.com/docs/intellij/publishing-plugin.html?from=IJPluginTemplate) for the first time.
- [ ] Set the Plugin ID in the above README badges.
- [ ] Set the [Deployment Token](https://plugins.jetbrains.com/docs/marketplace/plugin-upload.html).
- [ ] Click the <kbd>Watch</kbd> button on the top of the [IntelliJ Platform Plugin Template][template] to be notified about releases containing new features and fixes.
-->

<!-- Plugin description -->
# Yaksha Programming Language Plugin

#### Simple Syntax Highlighting
![Syntax Highlighting](https://raw.githubusercontent.com/YakshaLang/YakshaIntelliJ/main/screenshots/SyntaxHighLighting.png)

#### Structure View
![Structure](https://raw.githubusercontent.com/YakshaLang/YakshaIntelliJ/main/screenshots/Structure.png)

#### Hover docs (For standard library & auto generated from local code)
![Hover Docs](https://raw.githubusercontent.com/YakshaLang/YakshaIntelliJ/main/screenshots/HoverDocs.png)

#### Auto complete - imoports, . completion for imported libs
![Auto Complete](https://raw.githubusercontent.com/YakshaLang/YakshaIntelliJ/main/screenshots/AutoComplete.gif)

#### Find usages
![Find Usages](https://raw.githubusercontent.com/YakshaLang/YakshaIntelliJ/main/screenshots/FindUsages.png)

#### CTRL/CMD + Click navigation
![Click Jump](https://raw.githubusercontent.com/YakshaLang/YakshaIntelliJ/main/screenshots/ClickJump.gif)


🔗 [Yaksha Lang Documentation](https://yakshalang.github.io/)
<!-- Plugin description end -->

## Installation

- Download the [latest release](https://github.com/YakshaLang/YakshaIntelliJ/releases/latest) and install it manually using
  <kbd>Settings/Preferences</kbd> > <kbd>Plugins</kbd> > <kbd>⚙️</kbd> > <kbd>Install plugin from disk...</kbd>
  - You might need to uninstall current version if install fails
  - (Optional) Setup alternative colouring for operators/etc
    - Editor > Color Scheme > Yaksha
    - (Recommended)You should update the operator colour to something else
      - (Some themes does not display operator in a different colour)
- How to build manually
  - Open plugin in IntelliJ
  - Do a build (this takes a while first time)
  - Run gradle task buildPlugin
  - Find .zip file in build/distributions directory

---
Plugin based on the [IntelliJ Platform Plugin Template][template].

[template]: https://github.com/JetBrains/intellij-platform-plugin-template
