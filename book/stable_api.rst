Symfony2 ステーブル API
=======================

Symfony2 ステーブル API とは、リリース済みの Symfony2、およびコンポーネントやバンドルに含まれているすべてのパブリックメソッドのサブセットで、
次のルールに従います。

* 名前空間とクラス名は変更されません。
* メソッド名は変更されません。
* メソッドのシグネチャ（引数と戻り値の型）は変更されません。
* メソッドの振る舞いの意味は変更されません。

しかし、実装自体は変更される可能性はあります。ステーブル API の変更の唯一の正当な状況は、
セキュリティ問題を解決する場合のみです。

ステーブル API は、\ `@api` タグによるホワイトリスト方式で管理されています。
つまり、明示的にタグ付されていないものは、ステーブル API の一部ではないことを意味します。

.. tip::

    サードパーティーバンドルにおいても、バンドル自身のステーブル API を公開することを推奨します。

Symfony 2.0 では、次のコンポーネントで `@api` タグを使ったステーブル API が明示されています。

* BrowserKit
* ClassLoader
* Console
* CssSelector
* DependencyInjection
* DomCrawler
* EventDispatcher
* Finder
* HttpFoundation
* HttpKernel
* Locale
* Process
* Routing
* Templating
* Translation
* Validator
* Yaml

.. 2011/07/23 madapaja 6272eece5be43b5dca226c87672a0fce38b0bbf5
.. 2011/12/14 hidenorigoto 
