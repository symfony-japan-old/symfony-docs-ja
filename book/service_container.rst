.. index::
   single: Service Container
   single: Dependency Injection; Container

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/10/15

サービスコンテナ
================

モダンな PHP アプリケーションにはたくさんのオブジェクトがあります。あるオブジェクトが E メールメッセージの配信を容易にしている間に、別のオブジェクトは情報をデータベースに永続化できます。あなたのアプリケーションでは、プロダクト一覧を管理するオブジェクトを作成したり、サードパーティの API からのデータを処理するオブジェクトを作成できます。モダンなアプリケーション多くのことを行い、各タスクを扱うために多くのオブジェクトで構成されているのがポイントです。

この章では、インスタンス化を助け、アプリケーションの多くのオブジェクトを組み立て、取り出す Symfony2 における特別な PHP オブジェクトについて解説します。このオブジェクトはサービスコンテナと呼ばれ、アプリケーションを構成するオブジェクトを標準化し、一元化できるようになります。コンテナはエンジニアライフをより簡単に、すごく早くし、そして再利用可能で分離しているコードを促進しするアーキテクチャを重視させます。Symfony2 のコアなクラスはすべてコンテナを使用しているので、Symfony2 におけるあらゆるオブジェクトの拡張や設定、仕様の方法を学ぶことができます。大部分において、サービスコンテナは Symfony2 のスピードや拡張性に最も寄与しています。

最後に、サービスコンテナを設定、使用することは簡単です。本性の終りには、どんなサードパーティのバンドルからでもコンテナやカスタマイズオブジェクトを通して簡単に自分のオブジェクトを作成できるようになるでしょう。サービスコンテナが良いコードを簡単に書かせてくれるので、より再利用可能で、テスト可能で、分離可能なコードを書くようになるでしょう。

.. index::
   single: Service Container; What is a service?
   single: Service Container; What is a service container?

サービスとは何か？
------------------

シンプルに言うと、\ :term:`Service` (サービス) とは「グローバル」と分類されるタスクを実行するあらゆる PHP オブジェクトです。ある目的のため (メール配信など) のため作成されるオブジェクトを説明するため、コンピュータ科学で特定の意図を持って、また一般的に使用される名称です。各サービスはその提供する特定の機能を必要とされるときにアプリケーション内中を通して使用されます。サービスを作成するのに特別なことをする必要はありません。単純に特定のタスクを完遂するクラスとコードを書くだけです。おめでとう、あなたは今サービスを作成しました！

.. note::

   ルールとして、アプリケーションでグローバルに使われるオブジェクトである場合に、PHP オブジェクトはサービスとなります。グローバルに使用されるE メールメッセージを送信する為の単一の ``Mailer`` サービスに対し、送信される複数の ``Message`` オブジェクトはサービスでは\ *ありません*\ 。同じように、\ ``Product`` オブジェクトもサービスでありません。しかし、\ ``Product`` オブジェクトをデータベースに永続化するオブジェクトは\ *サービス* です。

さて、何がそんなに良いことなのでしょうか？「サービス」を考えることの利点は、アプリケーション中のサービスの種類ごとに機能の断片に分けて考えれるようになります。各サービスは１つだけの役割を行うので、必要なときに簡単に書くサービスにアクセスし、機能を使うことができます。各サービスはアプリケーション中で他の機能と分かれているので、簡単にテストや、設定ができるようにもなります。この考えは\ `サービス指向アーキテクチャ`_ と呼ばれていて、Symfony2 や PHP 固有のものではありません。独立したサービスクラスの集合でアプリケーションを構成することは、有名で、信用のあるオブジェクト指向のベストプラクティスです。どんな言語でも、これらのスキルは良い開発者になるためのキーです。

.. tip::

    この章を読み終えた後でより多くのこをを知りたいのであれば、
    :doc:`Dependency Injection Component Documentation</components/dependency_injection/introduction>`
    を読むとよいでしょう。


.. index::
   single: Service Container; What is?

サービスコンテナとは何か？
--------------------------

:term:`Service Container` (サービスコンテナ) (or *依存性注入コンテナ*) は単純にサービスのインスタンス化を管理する PHP オブジェクト (他の一般的なオブジェクトのように) です。
例えば、E メールメッセージを配信する単純な PHP クラスを考えてみます。サービスコンテナがなければ、必要なときにいつも手動でオブジェクトを作成しなければなりません。

::

    use Acme\HelloBundle\Mailer;

    $mailer = new Mailer('sendmail');
    $mailer->send('ryan@foobar.net', ...);

とても簡単です。この仮想の ``Mailer`` クラスは E メールメッセージを送信する (例えば\ ``sendmail``, ``smtp``, など) するためのメソッドの設定ができます。
ただ、他のどこかでも mailer サービスを使いたい時はどうでしょうか？ ``Mailer`` オブジェクトを使う必要があるときに\ *毎回*\ mailerの設定を繰り返すのは当然やりたくありません。アプリケーション中で出てくるたびに ``transport`` を ``sendmail`` や ``smtp`` に変更しなければならないとしたらどうでしょうか？すべての箇所を追い詰めて、\ ``Mailer`` クラスを作成、変更していかなければならないでしょう。

.. index::
   single: Service Container; Configuring services

コンテナ中でサービスを作成、設定する
------------------------------------

サービスコンテナに ``Mailer`` オブジェクトを作成させるのがベターな答えです。サービスコンテナを動作させるために、どのように ``Mailer`` オブジェクトを作成するか\ *教える*\ 必要があります。これは YAML, XML や PHP を通して詳細を設定します。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            my_mailer:
                class:        Acme\HelloBundle\Mailer
                arguments:    [sendmail]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="my_mailer" class="Acme\HelloBundle\Mailer">
                    <argument>sendmail</argument>
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('my_mailer', new Definition(
            'Acme\HelloBundle\Mailer',
            array('sendmail')
        ));

.. note::

   Symfony2 の初期化時に、アプリケーション設定を使用して(デフォルトでは ``app/config/config.yml``)サービスコンテナがビルドされます。
   実際に読み込まれるファイルは 環境独自のコンフィグレーションファイル (``config_dev.yml`` は ``dev`` 環境、¥ ``config_prod.yml`` は ``prod`` 環境のように) を読み込む ``AppKernel::registerContainerConfiguration()`` メソッドによって命令されます。

これで、サービスコンテナから ``Acme\HelloBundle\Mailer`` オブジェクトを利用できるようになりました。
コンテナは、通常の Symfony2 のコントローラから利用可能で、コンテナのサービスにアクセスするには、次のようにショートカットメソッドである ``get()`` を使います。

::

    class HelloController extends Controller
    {
        // ...

        public function sendEmailAction()
        {
            // ...
            $mailer = $this->get('my_mailer');
            $mailer->send('ryan@foobar.net', ...);
        }
    }

コンテナに対して ``my_mailer`` サービスを要求すると、コンテナによりオブジェクトが生成され、返されます。
これは、サービスコンテナを使う利点の 1 つでもあります。
つまり、実際に使う状況になるまで、サービスのオブジェクトが生成されることはありません。
定義したサービスをあるサービスでは利用しない場合、サービスのオブジェクトは作成されません。
これにより、メモリ使用量が低下し、アプリケーションの速度が向上します。
また、サービスの定義が増えたとしても、パフォーマンスにはほとんど影響を与えないことも意味します。
繰り返しますが、使われないサービスは、作成されないのです。

さらに、たとえば ``Mailer`` サービスをコンテナから取得する場合、最初の 1 回のみオブジェクトが生成され、それ以降は最初に生成されたのと同じインスタンスが返されます。
ほとんどの状況ではこの振る舞いをそのまま使えば良いのですが、もちろんさまざまなカスタマイズを加えることもできます。
また、同一のサービスオブジェクトを共有するのではなく、サービスの要求ごとに別々のインスタンスを作成するようにも設定できます。
":doc:`/cookbook/service_container/scopes`" で別々のインスタンスを作成する設定方法を学ぶことができます。

.. note::

    この例では、コントローラーはSymfonyのベースコントローラーを継承していてサービスコンテナに直接アクセスすることができます。
    だから ``get`` メソッドを使いサービスコンテナから ``my_mailer`` サービスを取得することができます。
    また\ :doc:`コントローラーをサービスとして</cookbook/controller/service>` 定義することもできます。
    やや高度な内容で必須のものではないのですが、コントローラーに必要なサービスだけを注入することができます。

.. _book-service-container-parameters:

サービスのパラメータ化
----------------------

コンテナによるサービス（たとえばオブジェクト）の作成は直線的に行われます。
サービスの定義にパラメータを使うと、管理しやすく柔軟になります。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        "%my_mailer.class%"
                arguments:    ["%my_mailer.transport%"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
                <parameter key="my_mailer.transport">sendmail</parameter>
            </parameters>

            <services>
                <service id="my_mailer" class="%my_mailer.class%">
                    <argument>%my_mailer.transport%</argument>
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

結果としては、以前のものと全く同じですが、サービスの定義方法が異なっている点に注意してください。
``my_mailer.class`` と ``my_mailer.transport`` をパーセント記号 (``%``) で囲むと、コンテナは、その名前のパラメータを探します。
コンテナが構築される際、パラメータの値が取得され、その値がサービスの定義に適用されます。

.. note::

    もしyamlファイルでパラメーターに ``@`` で始まる文字列を使いたい場合(例えば とても安全なメールパスワード)
    もう一つ ``@`` 記号を追加してエスケープする必要があります。(これはYAMLフォーマットのみ適用されます)

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            # This will be parsed as string "@securepass"
            mailer_password: "@@securepass"

.. note::

    パラメーターや引数内で文字列の一部にパーセント記号を使っている場合、
    もう一つパーセント記号を追加してエスケープしなければなりません

    .. code-block:: xml

        <argument type="string">http://symfony.com/?foo=%%s&bar=%%d</argument>

.. caution::

    ``request`` サービスを引数として渡した場合、
    :class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
    が発生するかもしれません。この問題についてより理解し解決する方法を学ぶためには、クックブックの記事
    :doc:`/cookbook/service_container/scopes`.
    を読むと良いでしょう。

パラメータを使うと、サービスに対して外から情報を与えることができます。
もちろん、パラメータを使わずに定義したサービスと、動作自体に違いはありません。
ですが、パラメータには次に挙げるようないくつかの利点があります。

* サービスのオプションを定義から分離し、\ ``parameters`` という単一のキー配下で管理できる。

* 複数のサービス定義で同じ値を重複して使っている場合でも、パラメータであれば複数のサービス定義で共有できる。

* すぐ後で解説するようにバンドル内でサービスを定義する時、パラメータを使った定義にしておくと、
  アプリケーション内でサービスをカスタマイズしやすくなります。

パラメータを使うかどうかは、開発者次第です。
クオリティの高いサードパーティのバンドルであれば、コンテナに登録されるサービスの設定変更を容易にするためにパラメータを使っていることでしょう。
ですが、アプリケーション内でのみ使うサービスであれば、パラメータを使った柔軟性が不要な場合もあります。

配列パラメーター(Array Parameters)
~~~~~~~~~~~~~~~~

パラメーターは配列も含むことができます。 :ref:`component-di-parameters-array` を参照して下さい。

別のコンテナコンフィギュレーションリソースをインポートする
----------------------------------------------------------

.. tip::

    この節では、サービスコンフィギュレーション・ファイルを\ *リソース*\ と呼びます。
    ほとんどのサービスコンフィギュレーションリソースは(YAML、XML、PHP といった)ファイルですが、Symfony2 はとてもフレキシブルなので
    (データベースや外部の Web サービスなど)どこからでもコンフィギュレーションを読み込むことができます。

サービスコンテナは１つのコンフィギュレーションリソース(デフォルトでは ``app/config/config.yml``) を使って組み立てられます。
(symfony2コアやサードパーティバンドルを含む)他の全てのサービスコンフィギュレーションはこのファイルから何らかの方法でインポートされなければなりません。
これによりあなたはアプリケーションにおいてサービスを超えたとても柔軟な設定が行えます。

異なる２つの方法で外部のサービスコンフィギュレーションを読み込むことができます。
1つめは、もっともよく使われる方法であり、 ``imports`` ディレクティブを通して行います。
以下の節では、２つ目の方法を紹介します。それは柔軟で、サードパーティバンドルから
サービスコンフィギュレーションをインポートするときに推奨される方法です。

.. index::
   single: Service Container; Imports

.. _service-container-imports-directive:

``imports`` を使ってコンフィギュレーションをインポートする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

これまで、私たちは ``my_mailer`` のサービスコンテナの定義を( ``app/config/config.yml`` といった)
アプリケーションコンフィギュレーションファイルに直接記述していました。もちろん、
``Mailer`` クラス自身は ``AcmeHelloBundle`` 内に存在しますが、 ``my_mailer`` コンテナの定義を
 バンドル内に入れた方がより良いでしょう。

初めに ``my_mailer`` コンテナ定義を ``AcmeHelloBundle`` 内の新しいコンテナリソースファイルに
移しましょう。もし ``Resources`` や ``Resources/config`` ディレクトリが存在していなければ作成して下さい。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        "%my_mailer.class%"
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
            <parameter key="my_mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                <argument>%my_mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

定義自体は変わらず配置場所だけが変わっています。もちろんサービスコンテナは新しいリソースファイルの存在を知りません。
ですが ``imports`` キーを使うことでリソースファイルを簡単に読み込むことができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: "@AcmeHelloBundle/Resources/config/services.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <imports>
                <import resource="@AcmeHelloBundle/Resources/config/services.xml"/>
            </imports>
        </container>

    .. code-block:: php

        // app/config/config.php
        $this->import('@AcmeHelloBundle/Resources/config/services.php');

``imports`` ディレクティブのおかげで、アプリケーションは任意の場所（通常bundle）にある
サービスコンテナコンフィギュレーションリソースを読み込む事ができます。
``リソース`` の場所は, ファイルの場合、リソースファイルへの絶対パスになります。
特別な ``@AcmeHello`` シンタックスは ``AcmeHelloBundle`` のディレクトリパスを解決します。
これにより、後から ``AcmeHelloBundle`` を異なるディレクトリに変更する場合にも気にせずに
リソースファイルのパスを記述することができます。

.. index::
   single: Service Container; Extension configuration

.. _service-container-extension-configuration:

コンテナエクステンションでコンフィギュレーションをインポートする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2で開発するときには、自身で特別に作ったバンドルからコンテナコンフィギュレーションを
インポートするために一般的に ``imports`` ディレクティブを使うことでしょう。
Symfony2のコアバンドルを含むサードパーティー製バンドルのコンテナコンフィギュレーションは、
通常、コンテナエクステンションというアプリケーションを設定するのにより柔軟かつ
簡単な別の仕組みでロードされます。

エクステンションの動作を簡単に説明しておきましょう。
内部的には、各バンドルはこれまで見てきたように非常に多くのサービスを定義しています。
すなわち、バンドルは、そのバンドルのパラメーターやサービスを指定するための１つ以上の
コンフィギュレーションリソースファイル（普通はXML）を使用します。
しかし、 ``imports`` ディレクティブを使用してアプリケーションコンフィギュレーションから
直接それらのリソースをインポートする代わりに、仕事を行うバンドル内部にある
*サービスコンテナエクステンション* を起動することができます。
サービスコンテナエクステンションはバンドル作成者が次の２つのことを行うために作るPHPクラスです。

* バンドルのサービスを設定するために必要な全てのサービスコンテナリソースをインポートする

* バンドルのサービスコンフィギュレーションのフラットなパラメータと相互にやり取りすること無く
  バンドルの設定ができるように、セマンティックで簡単な設定を提供する

言い換えれば、サービスコンテナエクステンションはあなたに代わってバンドルのサービスを設定します。
あなたがすぐに分かるようにと、エクステンションはバンドルを構成するための賢明で高度なインターフェースを提供します。

例として、Symfony2のコアフレームワークのバンドルである ``FrameworkBundle`` を持ってきました。
アプリケーションコンフィギュレーションにある以下のコードで ``FrameworkBundle`` 内の
サービスコンテナエクステンションは起動されます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            secret:          xxxxxxxxxx
            form:            true
            csrf_protection: true
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config secret="xxxxxxxxxx">
                <framework:form />
                <framework:csrf-protection />
                <framework:router resource="%kernel.root_dir%/config/routing.xml" />
                <!-- ... -->
            </framework>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'secret'          => 'xxxxxxxxxx',
            'form'            => array(),
            'csrf-protection' => array(),
            'router'          => array(
                'resource' => '%kernel.root_dir%/config/routing.php',
            ),

            // ...
        ));

コンフィギュレーションが解析されると、コンテナは ``framework`` コンフィギュレーション
ディレクティブを処理することのできるエクステンションを探します。
見つかったエクステンション（この場合は ``FrameworkBundle`` 内にあります）が起動され、
そして ``FrameworkBundle`` のサービスコンフィギュレーションは読み込まれます。
もし ``framework`` というキーをアプリケーションコンフィギュレーションファイルから完全に削除した場合、
Symfony2サービスは読み込まれません。
重要なのは「管理できる」という点です。Symfony2フレームワークにはいかなる魔法も制御できない機能もありません。

もちろん、もっと単純に ``FrameworkBundle`` のサービスコンテナエクステンションを
有効にすることができます。それぞれのエクステンションはサービスの内部がどのよう
に定義されているか気にすることなく、簡単にバンドルをカスタマイズできます。

この場合は ``error_handler``, ``csrf_protection``, ``router`` などがカスタマイズできます。
内部的には ``FrameworkBundle`` は、自身の特定のサービスを定義し設定するために、ここで指定されたオプションを使います。
バンドルはサービスコンテナに必要な全ての ``parameters`` と ``services`` の作成の面倒をみています。
まだ多くの構成を簡単にカスタマイズすることを可能にしながら。
さらに付け加えていうと、多くのサービスコンテナエクステンションはバリデーション機能を備えるほど優秀です。
オプションを忘れていたり、データ型が間違っている場合には知らせてくれます。

バンドルのインストールや設定時には、どのよにうにサービスをインストールし設定すべきか
バンドルのドキュメントを参照してください。コアのバンドルで利用可能なオプションは
:doc:`Reference Guide</reference/index>` で見ることができます。

.. note::

   元々は、サービスコンテナは ``parameters``, ``services``, ``imports``
   ディレクティブだけを認識します。その他のディレクティブはサービスコンテナ
   エクステンションによって扱われます。

もしバンドルの設定をユーザーフレンドリーになら、クックブック「:doc:`/cookbook/bundles/extension` 」を読みましょう

.. index::
   single: Service Container; Referencing services

サービスの参照（注入）
----------------------

これまでのところ ``my_mailer`` サービスはシンプルでした。 たった１つのコンストラクター
引数を受け取る、簡単な設定です。これから学んでいきますが、１つまたはそれ以上の他のサービスに
依存するサービスを作成するときに、コンテナの真の力に気がつくことでしょう。

例として、電子メールメッセージの作成とアドレスのコレクションにメール配信を管理する
のに役立つ、新しいサービス ``NewsletterManager`` があるとします。もちろん
``my_mailer`` はメールを配信において既にとても便利なので、 実際に配信する
メッセージをハンドリングするために ``NewsletterManager`` の内部で使います。
このクラスはこのようなものです。

::

    // src/Acme/HelloBundle/Newsletter/NewsletterManager.php
    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

サービスコンテナを使わなくても、かなり容易にコントローラー内で新しく
``NewsletterManager`` を作ることはできます。

::

    use Acme\HelloBundle\Newsletter\NewsletterManager;

    // ...

    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new NewsletterManager($mailer);
        // ...
    }

このアプローチは立派です。しかし、後から ``NewsletterManager`` に第二、第三のコンストラクター
引数を追加する必要がでてきた場合はどうでしょうか。コードをリファクタリングしたり、
クラスをリネームしたりする場合はどうでしょうか？いぜれのケースも ``NewsletterManager``
がインスタンス化されている場所を探し変更する必要があります。もちろん、
サービスコンテナにはさらなる追加オプションが存在します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: ["@my_mailer"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <!-- ... -->
                <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            </parameters>

            <services>
                <service id="my_mailer" ...>
                <!-- ... -->
                </service>
                <service id="newsletter_manager" class="%newsletter_manager.class%">
                    <argument type="service" id="my_mailer"/>
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer'))
        ));

YAMLの場合 ``@my_mailer`` シンタックスを使うことでコンテナは ``my_mailer``
と名付けられたサービスを探し、 ``NewsletterManager`` のコンストラクターに
オブジェクトを渡します。しかしながらこの場合、指定された ``my_mailer`` が存在して
いなれればなりません。もし存在していなければ例外が投げられます。
依存を任意のものとして印づけるけこともできます。
それについては次の節で説明することにします。

参照を使うことはとても強力な手段であり、依存が明確に定義された独自のサービスクラスを
作ることができます。この例では、 ``newsletter_manager`` サービスが機能するためには
``my_mailer`` サービスが必要です。サービスコンテナにこの依存を定義する時、
コンテナはオブジェクトのインスタンス作成における全ての仕事を処理します。

任意の依存性: セッターによる注入
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

コンストラクターで依存性を注入するやり方は、依存しているオブジェクトが利用可能な
状態であることを保証するのに優れた方法です。もしクラスに任意の依存を持たせたいの
であれば、セッターによる注入が良い方法かもしれません。
セッターによる注入とは、コンストラクターを通して行うのではなく、メソッド 呼び出し
を用いて依存を注入する方法のことを指します。

::

    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

セッターメソッドを用いた依存性注入は少しシンタックスを書き換えます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                calls:
                    - [setMailer, ["@my_mailer"]]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <!-- ... -->
                <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            </parameters>

            <services>
                <service id="my_mailer" ...>
                <!-- ... -->
                </service>
                <service id="newsletter_manager" class="%newsletter_manager.class%">
                    <call method="setMailer">
                        <argument type="service" id="my_mailer" />
                    </call>
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer'),
        ));

.. note::

    The approaches presented in this section are called "constructor injection"
    and "setter injection". The Symfony2 service container also supports
    "property injection".
    この節で紹介した方法はコンストラクターによる注入(コンストラクターインジェクション)
    セッターによる注入(セッターインジェクション)と呼ばれるものです。Symfony2のサービス
    コンテナはプロパティによる注入(プロパティインジェクション)もサポートしています。

参照を任意にする
----------------

場合によって、サービスは任意の依存を持っているかもしれません。それはきちんと
サービスが確実に動くことを必須としていないということを意味してます。
上の例では ``my_mailer`` は存在していなければなりませんでした。そうでなければ
例外が投げられることでしょう。 ``newsletter_manager`` サービスの定義を修正
することでこ参照を任意にすることができます。コンテナは ``my_mailer`` サービスが
存在していれば注入し、存在していなければなにもしません。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...

        services:
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: ["@?my_mailer"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="my_mailer" ...>
                <!-- ... -->
                </service>
                <service id="newsletter_manager" class="%newsletter_manager.class%">
                    <argument type="service" id="my_mailer" on-invalid="ignore" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference(
                    'my_mailer',
                    ContainerInterface::IGNORE_ON_INVALID_REFERENCE
                )
            )
        ));

Yamlの場合 ``@?`` シンタックスを用いることでコンテナに依存が任意であることを伝えます。
もちろん、任意の依存であることを許容するため ``NewsletterManager`` は書き直さなければ
なりません。

::

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Symfony コアバンドルとサードパーティバンドルのサービス
------------------------------------------------------

Symfony2や全てのサードパーティ製のバンドルは設定されコンテナ経由でサービスを
取得できるので、それらに簡単にアクセスすることができ自分のサービス内でそれらを
利用することもできます。物事をシンプルに保つために、Symfony2はデフォルトで
コントローラーをサービスとして定義することを必須としていません。更に言うと
Symfony2はサービスコンテナ全体がコントローラーに注入されています。
例として、ユーザーセッション上に情報保管をする方法をとり上げますが、
Symfony2は ``session`` サービスを提供していて、以下のようにスタンダード
コントローラーからアスセスすることができます。

::

    public function indexAction($bar)
    {
        $session = $this->get('session');
        $session->set('foo', $bar);

        // ...
    }

Symfony2では、Symfonyコアやサードパーティバンドルに提供された、テンプレートの
レンダリング (``templating``)、メール送信 (``mailer``)、 リクエスト情報へのアクセス
(``request``)といった各タスクを実行するためのサービスを何度も使うことでしょう。

さらにもうワンステップとして、自身のアプリケーション向けに作ったサービス内で
使うこともできます。
まず始めに、``NewsletterManager`` を修正して、Symfony2の本当の ``mailer`` サービスを(架空の
``my_mailer`` サービスの代わりに)そのまま使うようにしました。
また、テンプレートを通してメールの文章を作成できるように ``NewsletterManager``
にテンプレートエンジンサービスを渡すようにしました。

::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Templating\EngineInterface;

    class NewsletterManager
    {
        protected $mailer;

        protected $templating;

        public function __construct(
            \Swift_Mailer $mailer,
            EngineInterface $templating
        ) {
            $this->mailer = $mailer;
            $this->templating = $templating;
        }

        // ...
    }

サービスコンテナの設定は簡単です。

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: ["@mailer", "@templating"]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="mailer"/>
                <argument type="service" id="templating"/>
            </service>
        </container>

    .. code-block:: php

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference('mailer'),
                new Reference('templating'),
            )
        ));

``newsletter_manager`` サービスはコアの ``mailer`` と ``templating`` サービスを
利用できるようになりました。 これはフレームワーク内にある異なるサービスの力を活用し
アプリケーションに固有のサービスを作成するための一般的な方法です。

.. tip::

    アプリケーションの設定に ``swiftmailer`` の登録に表示されていることを確認してください。
    :ref:`service-container-extension-configuration` で述べられていますが、 ``swiftmailer`` キーは
    `SwiftmailerBundle`` からサービスエクステンションを呼び出し、 ``mailer`` サービスを登録します。

.. _book-service-container-tags:

タグ (``tags``)
~~~~~~~~~~~~~~~

Webのブログ記事に「Symfony」や「PHP」とタグ付けするのと同じで、コンテナに設定された
サービスにもタグをつけることができます。サービスコンテナにおいて、タグは
そのサービスが特定の目的のために使われることを表します。

.. configuration-block::

    .. code-block:: yaml

        services:
            foo.twig.extension:
                class: Acme\HelloBundle\Extension\FooExtension
                tags:
                    -  { name: twig.extension }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <service id="foo.twig.extension"
                class="Acme\HelloBundle\Extension\FooExtension">
                <tag name="twig.extension" />
            </service>
        </container>

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Extension\FooExtension');
        $definition->addTag('twig.extension');
        $container->setDefinition('foo.twig.extension', $definition);

``twig.extension`` タグは ``TwigBundle`` がコンフィギュレーション中に使う特別なタグです。
サービスに ``twig.extension`` タグをつけることでバンドル(TwigBundle)は
``foo.twig.extension`` サービスが、TwigのTwigエクステンションとして登録されるべきだ
 ということを知ります。言い換えると、Twigは ``twig.extension`` でタグ付けされた全ての
 サービスを探し、それら全てを自動的にTwigエクステンションとして追加します。

それゆえ、タグは自身のサービスを登録したり、またはバンドルによって特別な方法で
使われるよう、Symfony2やサードパーティ製のバンドルに指示する方法です。


以下はSymfony2のコアバンドルで利用可能なタグの一覧です。これらのタグは各々
サービスに対して異なる効果があり、多くのタグは追加の引数
(単なる ``name`` パラメーター以上のもの)を必要とします。

Symfony2のコアフレームワークで利用可能な全てのタグの一覧については
:doc:`/reference/dic_tags` を参照してください。

サービスのデバッグ
------------------

コンソールを使うことで、コンテナにどんなサービスが登録されているか見ることができます。
次のコマンドを実行することで全てのサービスとそのクラスが表示されます。

.. code-block:: bash

    $ php app/console container:debug

デフォルトではpublicなサービスが表示され、次のように --show-private オプション
つければprivateなサービスも表示させることができます。

.. code-block:: bash

    $ php app/console container:debug --show-private

また特定のサービスIDを指定することで、そのサービスについてより詳細な情報を得ることができます。

.. code-block:: bash

    $ php app/console container:debug my_mailer

もっと学ぶ
----------

* :doc:`/components/dependency_injection/parameters`
* :doc:`/components/dependency_injection/compilation`
* :doc:`/components/dependency_injection/definitions`
* :doc:`/components/dependency_injection/factories`
* :doc:`/components/dependency_injection/parentservices`
* :doc:`/components/dependency_injection/tags`
* :doc:`/cookbook/controller/service`
* :doc:`/cookbook/service_container/scopes`
* :doc:`/cookbook/service_container/compiler_passes`
* :doc:`/components/dependency_injection/advanced`

.. _`service-oriented architecture`: http://wikipedia.org/wiki/Service-oriented_architecture

.. 2011/07/22 shishi 55da9acdca0c74ab1b80a152c48b3f3d3e5eb62b
.. 2011/08/27 hidenorigoto 
.. 2012/10/13 okada 3f8ca1701dd4723ee107688471edc2f1316f1bd1
.. 2013/10/06 okapon(okada) f4aff75d06c5c341323752640b95101eaa4c1e29 (2.3)

