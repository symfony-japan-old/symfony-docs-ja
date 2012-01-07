.. index::
   single: CSS Selector

CssSelectorコンポーネント
=========================

    CssSelectorコンポーネントを使うと、CSSセレクタをXPath表現に置き換えることができます。

インストール
------------

コンポーネントをインストールする方法は何通りもあります。

* 公式Gitレポジトリ (https://github.com/symfony/CssSelector);
* PEARコマンドでインストール ( `pear.symfony.com/CssSelector`);
* Composerを使ってインストール (Packagistの `symfony/css-selector`).

使い方
-------

コンポーネントの唯一の目的はCSSセレクタをXPath表現に変換することです
::

    use Symfony\Component\CssSelector\CssSelector;

    print CssSelector::toXPath('div.item > h4 > a');
