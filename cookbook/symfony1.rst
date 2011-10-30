symfony1 ユーザーのための Symfony2
==================================

Symfony2 は、 symfony1 と比べると、 著しい進化が取り込まれています。
幸い、 Symfony2 の中核は MVC アーキテクチャーで、 symfony1 のプロジェクトをマスターするために使用した技術は、 Symfony2 で開発するときにも十分意味があります。
確かに、 ``app.yml`` は無くなりましたが、ルーティングやコントローラーやテンプレートは、全て残っています。

このガイドでは、 symfony1 と Symfony2 の違いを概観していきましょう。
見ていくとわかりますが、多くの機能は少々異なった方法で取り組まれています。
このちょっとした違いによって、 Symfony2 アプリケーションのコードの、安定性、予測可能性、テスト可能性、独立性が増すことに感謝するようになるでしょう。

さて、それでは "過去" から "今" への旅をしましょう。

ディレクトリ構造
----------------

Symfony2 のプロジェクト (例えば `Symfony2 Sandbox`_) を見ると、 symfony1 とは非常に異なったディレクトリ構造であることに気づくでしょう。しかし、この違いは表面的なものにすぎません。

``app/`` ディレクトリ
~~~~~~~~~~~~~~~~~~~~~

symfony1 では、プロジェクトに一つあるいは複数のアプリケーションがあり、それぞれが ``apps/`` ディレクトリ以下にありました (例えば ``apps/frontend``) 。 Symfony2 のデフォルトでは、 ``app/`` ディレクトリに、一つだけアプリケーションを保持することになります。 symfony1 のように、 ``app/`` ディレクトリにはアプリケーションに独自のコンフィギュレーションが置かれます。また、アプリケーション毎のキャッシュ、ログそしてテンプレートのディレクトリがあり、アプリケーションを表す基本オブジェクトである ``Kernel`` クラス (``AppKernel``) も存在します。

symfony1 とは異なり、 ``app/`` ディレクトリに PHP のコードは、ほとんどありません。このディレクトリは、モジュールやライブラリファイルを格納するためのものではないのです。そうではなく、このディレクトリは単にコンフィギュレーションと他のリソース (テンプレートや翻訳ファイル) の置き場所です。

``src/`` ディレクトリ
~~~~~~~~~~~~~~~~~~~~~

単純に言えば、実際のコードはこのディレクトリに入ります。
Symfony2 では、すべての実際のアプリケーションのコードはバンドル (大雑把に言うと symfony1 のプラグインのようなもの) の中にあり、デフォルトでは、個々のバンドルは ``src`` ディレクトリの中にあります。このように、 ``src`` ディレクトリは symfony1 での ``plugins`` ディレクトリに似たものですが、しかし、それよりもかなりフレキシブルです。付け加えると、 *あなたの* バンドルは ``src/`` ディレクトリに有りますが、サードパーティのバンドルは、 ``vendor/`` ディレクトリにあるでしょう。

``src/`` ディレクトリをより理解するために、 symfony1 のアプリケーションを思い浮かべてみましょう。まず、あなたのコードは一つあるいは複数のアプリケーションの中にあるでしょう。一般的には、これらのアプリケーションはモジュールを含んでいます。あなたがアプリケーションに加えた他の PHP クラスもあるかもしれません。 ``schema.yml`` ファイルをプロジェクトの ``config`` ディレクトリに作ったり、モデルファイルを作ったりするでしょう。また、よくある機能を実現するためにサードパーティのプラグインを使っているならば、それらは ``plugins/`` ディレクトリにあるでしょう。つまり、あなたのアプリケーションを機能させるためのコードは、多くの場所に分かれて置かれているということです。

Symfony2 では、状況はもっと簡単になります。というのも、 *全ての* Symfony2 のコードは、バンドルの中にあるからです。いま思い浮かべていただいた symfony1 のプロジェクトでのコードのすべては、一つあるいは複数のプラグインに移すことが *できる* でしょう (そして、それは良い練習になるでしょう) 。全てのモジュール、 PHP クラス、スキーマ、ルーティング設定、その他諸々がプラグインに移動できたとすると、 symfony1 での ``plugins/`` ディレクトリは Symfony2 での ``src/`` ディレクトリと非常によく似たものになるでしょう。

再び単純に言うならば、 ``src/`` ディレクトリが、あなたのコード、アセット、テンプレートおよびその他のプロジェクトに特有のものが置かれる場所なのです。

``vendor/`` ディレクトリ
~~~~~~~~~~~~~~~~~~~~~~~~

``vendor/`` ディレクトリは、基本的には symfony1 での ``lib/vendor/`` ディレクトリと同等のものです。すべてのベンダーライブラリーが置かれる場所となります。デフォルトでは、 Symfony2 のライブラリーファイルは、 Doctrine2 、 Twig や Swiftmailer 等の Symfony2 が使用するいくつかのライブラリーとともに、このディレクトリに置かれます。

``web/`` ディレクトリ
~~~~~~~~~~~~~~~~~~~~~

``web/`` ディレクトリにはあまり変更がありません。一番目立った違いは、 ``css/`` 、 ``js/`` および ``images/`` ディレクトリがないことでしょう。これは、意図的なものです。 PHP コード同様、すべてのアセットはバンドルの中にあります。コンソールコマンドを使って、個々のバンドルの ``Resources/public/`` ディレクトリの複製あるいはシンボリックリンクが ``web/bundles/`` ディレクトリに作られます。つまり、バンドル内にアセットを置いておいてあっても、パブリックでも利用可能にすることができます。すべてのバンドルのアセットを、利用可能にするには、次のコマンドを実行いてください。

    php app/console assets:install web

.. note::

   このコマンドは symfony1 での ``plugin:publish-assets`` コマンドに相当します。

オートローディング
------------------

現代的なフレームワークの利点の一つとして、必要とされるファイルについて気に病む必要がないことです。オートローダーを使うことによって、プロジェクト内の任意のクラスを参照でき、またそれが利用可能であるとが信頼できます。オートローディングは Symfony2 で、より普遍的に、より速く、そしてキャッシュのクリアーが必要ないように、変更されました。

symfony1 では、プロジェクト全体の PHP クラスファイルを検索してその情報を一つの巨大な配列にキャッシュすることによって、オートロードを実現していました。この配列によって symfony1 は個々のクラスがどのファイルに含まれているのかを、正確に知ることができました。そのため、プロダクション環境においては、クラスを追加したり移動したりした場合にキャッシュをクリアする必要がありました。

Symfony2 では、 ``UniversalClassLoader`` という新たなクラスがこの仕組みを担います。このオートローダーの考え方は単純です。クラスの名前 (ネームスペースを含む) は、そのクラスを含むファイルのパスに一致していなければならないというものです。一例として Symfony2 の Standard Edition から ``FrameworkExtraBundle`` を取り上げてみましょう::

    namespace Sensio\Bundle\FrameworkExtraBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;
    // ...

    class SensioFrameworkExtraBundle extends Bundle
    {
        // ...

ファイルそのものは ``vendor/bundle/Sensio/Bundle/FrameworkExtraBundle/SensioFrameworkExtraBundle.php`` にあります。これからわかるように、ファイルの場所は、クラスのネームスペースに従います。具体的には、ネームスペース ``Sensio\Bundle\FrameworkExtraBundle`` がそのままファイルが存在するディレクトリとなります (``vendor/bundle/Sensio/Bundle/FrameworkExtraBundle``) 。これは次のように ``app/autoload.php`` ファイルに  ``vendor/bundle`` ディレクトリ内の ``Sensio`` ネームスペースを探すように設定しているからです。

.. code-block:: php

    // app/autoload.php

    // ...
    $loader->registerNamespaces(array(
        // ...
        'Sensio'           => __DIR__.'/../vendor/bundles',
    ));

ファイルがこの場所になかった場合には、 ``Class "Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBUndle" は存在しません。`` というエラーが発生するでしょう。 Symfony2 では、 "クラスが存在しない" というのは期待されたクラスのネームスペースと物理的な位置とが一致しないということを意味します。これは、 Symfony2 が目的のクラスの唯一正確な位置だけを見るのですが、その位置に目的のクラスが存在しなかった (または異なるクラスが含まれている) ということです。クラスをオートロードするために、 Symfony2 では **キャッシュをクリアする必要はありません** 。

しかしながら、オートローダーが機能するために、例えば、 ``Sensio`` というネームスペースが ``vendor/bundles`` ディレクトリにあり、 ``Doctrine`` というネームスペースが ``vendor/doctrine/lib/`` ディレクトリにある、ということをオートローダーが知っている必要があります。このマッピングは、 ``app/autoload.php`` ファイルによって一元的に制御されます。

Symfony2 Standard Edition の ``HelloController`` を見てみると、 そのコントローラが ``Acme\DemoBundle\Controller`` ネームスペースにあることがわかります。しかし、 ``Acme`` ネームスペースはまだ ``app/autoload.php`` には定義されていません。デフォルトでは、 ``src/`` ディレクトリ内にあるバンドルの場所は明示的に設定する必要はありません。 ``UniversalClassLoader`` が ``registerNamespacesFallbaacks`` メソッドを使い ``src/`` ディレクトリをフォールバックするように設定されています。

.. code-block:: php

    // app/autoload.php

    // ...
    $loader->registerNamespaceFallbacks(array(
        __DIR__.'/../src',
    ));

コンソールを使う
----------------

symfony1 で、コンソールはプロジェクトのルートディレクトリにあり、 ``symfony`` という名前でした:

.. code-block:: text

    php symfony

Symfony2 では、コンソールは app サブディレクトリにあり、 ``console`` という名前になっています:

.. code-block:: text

    php app/console

アプリケーション
----------------

symfony1 のプロジェクトでは、通常、複数のアプリケーションがあります: 例えば、 frontend と backend などです。

Symfony2 のプロジェクトでは、ただひとつのアプリケーションをつくるだけで済みます (ブログアプリケーションだったり、イントラネットアプリケーションだったり、 ...) 。通常、二つ目のアプリケーションを作りたいと思ったら、別のプロジェクトを作りそれらの間でバンドルを共有することになるでしょう。

バンドルの frontend の機能と backend の機能を分ける必要があるならば、コントローラに関してはサブネームスペースを、テンプレートに関してはサブディレクトリを、それぞれに対応したコンフィギュレーション、別個のルーティング設定、などなどを作ることができます。

もちろん、プロジェクトに複数のアプリケーションを持たせても何も悪くはありません。すべてはあなたの自由です。二つ目のアプリケーションは ``app/`` ディレクトリと基本のコンフィギュレーションを同じくする新しいディレクトリ (例えば ``my_app/``) となるでしょう。

.. tip::

    用語集の :term:`プロジェクト` 、 :term:`アプリケーション` 、 そして :term:`バンドル` の定義を読んでください。

バンドルとプラグイン
--------------------

symfony1 のプロジェクトで、コンフィグレーション、モジュール、 PHP ライブラリー、アセットおよびその他プロジェクトに関係したアセットが、プラグインに含まれています。 Symfony2 では、プラグインの考え方は "バンドル" に取って代られました。バンドルはプラグインよりもずっと強力なので、 Symfony2 のコアフレームワークは、一連のバンドルによって実現されています。 Symfony2 では、バンドルがまず第一であり、バンドルは非常にフレキシブルであるので Symfony2 のコアコード自体もバンドルになっています。

symfony1 では、プラグインは ``ProjectConfiguration`` で有効化されなければなりませんでした::

    // config/ProjectConfiguration.class.php
    public function setup()
    {
        $this->enableAllPluginsExcept(array(/* some plugins here */));
    }

Symfony2 で、バンドルはアプリケーションカーネルの中でアクティベートされます::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            // ...
            new Acme\DemoBundle\AcmeDemoBundle(),
        );

        return $bundles;
    }

ルーティング (``routing.yml``) とコンフィギュレーション (``config.yml``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

symfony1 では、 ``routing.yml`` と ``app.yml`` は、任意のプラグインで自動的にロードされます。 Symfony2 では、バンドルの中のルーティングおよびアプリケーションのコンフィギュレーションは、手動でインポートしなければなりません。例えば、 ``AcmeDemoBundle`` からルーティングの情報をインポートするには、以下のようにする必要があるでしょう::

    # app/config/routing.yml
    _hello:
        resource: "@AcmeDemoBundle/Resources/config/routing.yml"

``AcmeDemoBundle`` の ``Resources/config/routing.yml`` ファイルにあるルーティングの設定をロードします。 ``@AcmeDemoBundle`` は特別なショートカットシンタックスで、内部で指定のバンドルへのフルパスに変換しています。

全く同じようにコンフィギュレーションをバンドルからロードすることができます。

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: "@AcmeDemoBundle/Resources/config/config.yml" }

Symfony2 では、コンフィギュレーションは symfony1 の ``app.yml`` に少しだけ似ています。しかし、 ``app.yml`` では、どんなキーも作るとができたのに比べて、 Symfony2 のコンフィギュレーションの方がもっとシステマティックです。 ``app.yml`` では、エントリは特に意味があるわけではなく、開発者の意向でアプリケーションでどう使うかを決めてました。

.. code-block:: yaml

    # some app.yml file from symfony1
    all:
      email:
        from_address:  foo.bar@example.com

Symfony2 では、コンフィギュレーションの ``parameters`` キーの下に symfony1 で定義していた ``app.yml`` のような自由なエントリを作ることができます。

.. code-block:: yaml

    parameters:
        email.from_address: foo.bar@example.com

これで以下のようにコントローラからアクセスできるようになりました。

    public function helloAction($name)
    {
        $fromAddress = $this->container->getParameter('email.from_address');
    }

実際には、 Symfony2 のコンフィギュレーションはより強力で、主に使用するオブジェクトを設定するために使われます。詳細は、\ ":doc:`/book/service_container`" の章を参照してください。

.. _`Symfony2 Standard`: https://github.com/symfony/symfony-standard

.. 2011/10/23 ganchiku c31ead51512c373dbcb9c32b1f33d01037e0682e

