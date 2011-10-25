.. index::
   single: Bundles; Best Practices

バンドルの構造とベストプラクティス
===================================

バンドルは、洗練された構造をしており、クラスファイルからコントローラやウェブのリソースまで何でも格納できるディレクトリです。バンドルはとてもフレキシブルですが、バンドルを配布する際にはベストプラクティスに沿う必要があります。

.. index::
   pair: Bundles; Naming Conventions

.. _bundles-naming-conventions:

バンドル名
-----------

１つバンドルは同時に１つの PHP のネームスペースでもあります。ネームスペースは、 PHP5.3 のネームスペースとクラス名の技術的な相互運用の `standards_` に沿う必要があります。つまり、ベンダーの区分から始まり、カテゴリの区分(無い場合もあるし、複数ある場合もあります)が続き、必ず ``Bundle`` という接尾辞で終わる短いネームスペースになります。

バンドルクラスを追加すると同時にネームスペースはバンドルになります。バンドルのクラス名は次のルールに沿う必要があります。:

* 半角英数字、アンダースコアのみ使用すること
* ネーミングには、キャメルケースを使用すること
* 意味のある短い名前を使うこと(短い方が望ましく、多くとも二語とする)
* ベンダーに連続する名前を前に置くこと(カテゴリのネームスペースがある際には、それに連続すること)
* ``Bundle`` 接尾辞を付けること

以下に有効なネームスペースとクラス名の例を示します。:

+-----------------------------------+--------------------------+
| ネームスペース                    | バンドルのクラス名       |
+===================================+==========================+
| ``Acme\Bundle\BlogBundle``        | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+
| ``Acme\Bundle\Social\BlogBundle`` | ``AcmeSocialBlogBundle`` |
+-----------------------------------+--------------------------+
| ``Acme\BlogBundle``               | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+

慣例により、バンドルクラスの ``getName()`` メソッドはクラス名を返す必要があります。

.. note::

    バンドルをパブリックに共有する際には、バンドルのクラス名をリポジトリの名前として使用するようにしなければなりません。例えば、 ``BlogBundle`` ではなく、``AcmeBlogBundle`` としてください。

.. note::

    Symfony2 のコアのバンドルは、バンドルのクラス名に ``Symfony`` という名前を前に置いておらず、常にサブネームスペースである ``Bundle`` を追加しています。例えば、 :class:`Symfony\\Bundle\\FrameworkBundle\\FrameworkBundle` のようにです。

それぞれのバンドルの名前には、小文字とアンダースコアを使用して短くしたエイリアスがあります。例えば、 ``AcmeHelloBundle`` は ``acme_hello`` になり、 ``Acme\Social\BlogBundle`` は ``acme_social_blog`` になります。エイリアスは、同バンドル内で一意である必要があります。以下のセクションで使用方法の例を示します。

ディレクトリ構造
-------------------

``HelloBundle`` バンドルのベーシックなディレクトリ構造は以下の通りです。:

.. code-block:: text

    XXX/...
        HelloBundle/
            HelloBundle.php
            Controller/
            Resources/
                meta/
                    LICENSE
                config/
                doc/
                    index.rst
                translations/
                views/
                public/
            Tests/

``XXX`` ディレクトリは、バンドルのネームスペースの構造を示しています。

以下のファイルは必ず必要です。:

* ``HelloBundle.php``;
* ``Resources/meta/LICENSE``: バンドル内のコードの完全なライセンス
* ``Resources/doc/index.rst``: バンドルのドキュメントのためのルートファイル

.. note::

    これらの慣習は、デフォルトの構造に依存している自動化ツールが、ちゃんと動作するためのものです。

使用頻度の高いクラスファイル等を保管するサブディレクトリの深さは、最小に抑えるべきです。最大でも二階層にしてください。あまり重要でなかったり、使用頻度の低いファイルは、階層を深いところに置いてもいいでしょう。

バンドルディレクトリの権限は、リードオンリーにします。もし、一時ファイルを書き込む必要があるなら、アプリケーションの ``cache/`` や ``log/`` ディレクトリに保管してください。自動化ツールは、バンドルのディレクトリ構造内にファイルを生成することができますが、それは、生成されたファイルがリポジトリの一部となるときのみです。

以下のクラスとファイルは、特定の位置に配置されます。:

+------------------------------+-----------------------------+
| Type                         | Directory                   |
+==============================+=============================+
| Commands                     | ``Command/``                |
+------------------------------+-----------------------------+
| Controllers                  | ``Controller/``             |
+------------------------------+-----------------------------+
| Service Container Extensions | ``DependencyInjection/``    |
+------------------------------+-----------------------------+
| Event Listeners              | ``EventListener/``          |
+------------------------------+-----------------------------+
| Configuration                | ``Resources/config/``       |
+------------------------------+-----------------------------+
| Web Resources                | ``Resources/public/``       |
+------------------------------+-----------------------------+
| Translation files            | ``Resources/translations/`` |
+------------------------------+-----------------------------+
| Templates                    | ``Resources/views/``        |
+------------------------------+-----------------------------+
| Unit and Functional Tests    | ``Tests/``                  |
+------------------------------+-----------------------------+

クラス
-------

バンドルディレクトリの構造は、ネームスペースの階層として使用されます。例えば、 ``HelloController`` コントローラは、 ``Bundle/HelloBundle/Controller/HelloController.php`` に保管されますし、完全なクラス名は、 ``Bundle\HelloBundle\Controller\HelloController`` になります。

全てのクラスとファイルは、 Symfony2 の基準に沿う必要があります。:doc:`standards </contributing/code/standards>`

クラスのいくつかは、わかりやすくするために、可能な限り短くあるべきです。例えば、 Commands, Helpers, Listeners, Controllers などです。

イベントディスパッチャーに接続するクラスは、 ``Listener`` の接尾辞をつけるべきです。

例外クラスは、 ``Exception`` のサブネームスペース内に保管するべきです。

ベンダー
-------

バンドルは、サードパーティの PHP ライブラリを含んではいけません。その代わりに、標準的な Symfony2 のオートローディングを使いましょう。

バンドルは、サードパーティの JavaScript  や CSS などのライブラリを含んではいけません。

テスト
-----

バンドルは、 ``Tests/`` ディレクトリ以下に PHPUnit で書かれたテストスイートを用意すべきです。テストは以下の原則に沿ってください。:

* テストスイートは、サンプルアプリケーションから ``phpunit`` コマンドのみを走らせるだけで実行できなければなりません。
* 機能テストは、レスポンスの出力とプロファイル情報のみをテストすべきです。
* コードカバレッジは、コードベースの少なくとも 95% 以上にすべきです。

.. note::
   テストスイートに ``AllTests.php`` スクリプトを含んまないでください。代わりに ``phpunit.xml.dist`` ファイルを使用してください。

ドキュメント
-------------

全てのクラスと関数は PHPDoc が必要です。

詳細なドキュメントを用意する際は、 ``Resources/doc/`` ディレクトリ以下に、 :doc:`reStructuredText </contributing/documentation/format>` フォーマットで用意してください。 ``Resources/doc/index.rst`` ファイルは唯一必要なファイルで、詳細なドキュメントへのエントリポイントとしてください。

コントローラ
-----------

ベストプラクティスとして、他人に配布することを考慮する必要があるので、バンドル内のコントローラは、 :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` のベースクラスを拡張してはなりません。代わりに、　:class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface` インタフェースを実装するか、  :class:`Symfony\\Component\\DependencyInjection\\ContainerAware` クラスを拡張してください。

.. note::

    :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` クラスのメソッドは、単に学習を簡単にさせるショートカットの集合です。

ルーティング
-------

バンドルでルーティングを提供するには、ルート名にバンドルエイリアスの接頭辞を付ける必要があります。例えば、 ``AcmeBlogBundle`` であれば、全てのルート名は、 ``acme_blog_`` の接頭辞を必ず付けてください。

テンプレート
---------

バンドルがテンプレートを提供する際には、 Twig を必ず使ってください。バンドルは、単体で動くアプリケーションでなければ、メインとなるレイアウトを用意するべきではありません。

翻訳ファイル
-----------------

バンドルが翻訳ファイルを提供する際には、 XLIFF フォーマットで定義する必要があります。そして、XLIFF のメッセージドメインは、バンドル名(例えば ``bundle.hello``)の後に名前を付けてください。

バンドルは、他のバンドルで使われているメッセージを上書きしてはなりません。

コンフィギュレーション
-------------

さらにフレキシブルにするために、 Symfony2 のビルトインメカニズムを使用すれば、バンドルはコンフィギュレーションの設定も提供することができます。

簡単なコンフィギュレーションの設定は、Symfony2 のコンフィギュレーションのデフォルトの ``parameters`` のエントリに依存しています。Symfony2 のパラメターは、簡単なキーとバリューのペアになります。バリューは、 PHP で有効な値になります。それぞれのパラメター名は、バンドルエイリアスの接頭辞を付ける必要があります。ただし、これは単にベストプラクティスとしての提案です。パラメター名の残りは、異なる部分のセパレータとして、ピリオド (``.``)を使うことになります。例えば、 ``acme_hello.email.from`` のようになります。

以下のように、エンドユーザはどんなコンフィギュレーションファイル内の値も規定することができます。:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            acme_hello.email.from: fabien@example.com

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="acme_hello.email.from">fabien@example.com</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('acme_hello.email.from', 'fabien@example.com');

    .. code-block:: ini

        [parameters]
        acme_hello.email.from = fabien@example.com

コードでは、コンテナから設定パラメターを取得します。::

    $container->getParameter('acme_hello.email.from');

このメカニズムはとてもシンプルですが、他のクックブックの記事( :doc:`/cookbook/bundles/extension`) にあるように意味的なコンフィギュレーションを使用した方が良いえしょう。

.. note::

    サービスを定義する際は、バンドルエイリアスの接頭辞を付ける必要があります。

さらなる詳細は、クックブックの他の記事も参考としましょう。
----------------------------

* :doc:`/cookbook/bundles/extension`

.. _standards: http://groups.google.com/group/php-standards/web/psr-0-final-proposal
