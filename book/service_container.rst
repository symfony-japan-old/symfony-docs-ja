.. index::
   single: Service Container
   single: Dependency Injection; Container

.. note::

    * 対象バージョン：2.3、2.4
    * 翻訳更新日：2013/10/19

サービスコンテナ
================

一般的にアプリケーションは、さまざまなオブジェクトで構成されています。電子メールメッセージの送信を担当するオブジェクトがあれば、情報のデータベースへの永続化を担当するオブジェクトもあります。アプリケーション内でも、製品の在庫管理を担当するオブジェクトがあれば、外部 API から取得したデータの処理を担当するオブジェクトもあります。アプリケーションには多くの仕事がありますが、個々の仕事はそれを専門で担当する小さなオブジェクトに割り当てられ、それが集まってアプリケーションが構成されているのです。

この章では、Symfony2 のサービスコンテナについて解説します。サービスコンテナは 1 つの PHP オブジェクトであり、アプリケーション内でオブジェクトをインスタンス化、組み立て、取り出すのに利用します。サービスコンテナを使うと、アプリケーションにおけるオブジェクトの生成方法を標準化し、一元化できるようになります。再利用可能で疎結合なコードのためのアーキテクチャを提供しています。Symfony2 のすべてのコアクラスはコンテナを使っているので、Symfony2 の任意のオブジェクトを拡張し、設定して利用する方法もこの章で学べます。サービスコンテナは、Symfony2 のプロダクション環境での実行速度と、コードの拡張性に大きく貢献しています。

サービスコンテナのコンフィギュレーションや利用は難しくありません。本章を最後まで読めば、アプリケーション用に定義したオブジェクトをコンテナ経由でインスタンス化したり、サードパーティのバンドルで提供されるオブジェクトをカスタマイズして利用できるようになります。

.. tip::

    この章を読み終えた後、さらに詳しく学びたい方は、
    :doc:`Dependency Injection コンポーネント </components/dependency_injection/introduction>`
    のドキュメントを参照してください。

.. index::
   single: Service Container; What is a service?

サービスとは何か？
------------------

シンプルに言うと、\ :term:`Service` (サービス) とは何らかのタスクを実行する PHP オブジェクトです。ある目的 (メール配信など) のために作成されるオブジェクトを指す、コンピュータサイエンスで一般的に使用される用語です。それぞれのサービスは、アプリケーションでその機能が必要になった時点で利用されます。サービスを作成するために特別な作業は必要ありません。目的の仕事を行う PHP クラスのコードを書けば、それがサービスクラスそのものです。

.. note::

   アプリケーション全体で使われるオブジェクトはサービスです。電子メールメッセージの送信を担当する単一の ``Mailer`` サービスはアプリケーション全体で使われますが、送信するそれぞれの ``Message`` オブジェクトはサービスでは\ *ありません*\ 。同じように、\ ``Product`` オブジェクトもサービスでありません。しかし、\ ``Product`` オブジェクトをデータベースへ永続化するオブジェクトは\ *サービス*\ です。

サービスの利点は何でしょうか？ サービスを中心に考えると、アプリケーションの機能をサービスに分割して扱えるようになります。各サービスの役割は1つだけで単純にしておき、必要になった時にサービスにアクセスしてその機能を使う、という考え方でアプリケーションを構築できます。個々のサービスはアプリケーションの他の機能から分離されているので、テストしやすく、構成も簡単です。この考え方は\ `サービス指向アーキテクチャ`_ と呼ばれ、Symfony2 独自のものではありません。独立したサービスクラスの集合でアプリケーションを構成することは、一般的で信頼性のあるオブジェクト指向設計・プログラミングのベストプラクティスです。

.. index::
   single: Service Container; What is a service container?

サービスコンテナとは何か？
--------------------------

:term:`Service Container` (サービスコンテナ) (または *依存性注入コンテナ*) はサービスのインスタンス化を管理する単純な PHP オブジェクトです。
例えば、電子メールメッセージを配信する単純な PHP クラスを考えてみます。サービスコンテナがない場合、メールを送りたい箇所でオブジェクトを作成するコードを毎回記述しなくてはなりません。

::

    use Acme\HelloBundle\Mailer;

    $mailer = new Mailer('sendmail');
    $mailer->send('ryan@foobar.net', ...);

これだけであれば、とても簡単です。この仮の ``Mailer`` クラスは、電子メールメッセージの具体的な送信方法 (例えば ``sendmail``\ 、\ ``smtp`` など) をコンストラクタの引数で指定できるようになっています。
しかし、アプリケーションの別の箇所で mailer サービスを使いたい時はどうでしょうか？ ``Mailer`` オブジェクトを使う必要があるときに\ *毎回* mailer の設定を繰り返すことはしたくありません。\ ``Mailer`` のコンストラクタ引数で指定している ``transport`` を ``sendmail`` から ``smtp`` に変更しなければならないとしたらどうでしょうか？ サービスをインスタンス化している箇所をすべて調べて変更していかなければならないでしょう。

.. index::
   single: Service Container; Configuring services

コンテナ中でサービスを生成・構成する
------------------------------------

``Mailer`` オブジェクトをサービスコンテナで管理すれば問題は解決します。サービスコンテナで管理するには、\ ``Mailer`` オブジェクトの生成方法（コンフィギュレーション）をサービスコンテナに伝える必要があります。YAML、XML、PHP 形式でオブジェクトのコンフィギュレーションを記述できます。

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

   Symfony2 の初期化時に、アプリケーションコンフィギュレーション(デフォルトでは ``app/config/config.yml``)を使ってサービスコンテナがビルドされます。
   実際に読み込まれるファイルは ``AppKernel::registerContainerConfiguration()`` メソッドで指定されます。
   環境ごとのコンフィギュレーションファイル (``dev`` 環境では ``config_dev.yml``\ 、\ ``prod`` 環境では ``config_prod.yml``\ ) が読み込まれるようになっています。

これで、サービスコンテナから ``Acme\HelloBundle\Mailer`` オブジェクトを利用できるようになりました。
コンテナは、通常の Symfony2 のコントローラから利用可能です。コンテナ中のサービスにアクセスするには、次のようにショートカットメソッドである ``get()`` を使います。

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

    この例では、コントローラーは Symfony のベースコントローラーを継承していてサービスコンテナに直接アクセスすることができます。
    だから ``get`` メソッドを使いサービスコンテナから ``my_mailer`` サービスを取得することができます。
    また\ :doc:`コントローラーをサービスとして </cookbook/controller/service>`\ 定義することもできます。
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

    もし YAML ファイルでパラメーターに ``@`` で始まる文字列を使いたい場合(例: とても安全なメールパスワード)
    もう一つ ``@`` 記号を追加してエスケープする必要があります。(これは YAML フォーマットのみに適用されます)

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


サービスコンテナは１つのコンフィギュレーションリソース(デフォルトでは ``app/config/config.yml``\ )を使って組み立てられます。
リソースファイル中から(symfony2 コアやサードパーティバンドルを含む)他の全てのサービスコンフィギュレーションを何らかの方法で読み込まなければなりません。
この方法により、アプリケーション内のサービスをとても柔軟に設定することができます。

外部のサービスコンフィギュレーションを読み込む方法はは２つあります。
1つめは、もっともよく使われる ``imports`` ディレクティブを介して行う方法です。
2つめは、コンテナエクステンションを使う方法です。2つめの方法はとても柔軟で、
サードパーティバンドルからサービスコンフィギュレーションを読み込む場合に推奨されています。

.. index::
   single: Service Container; Imports

.. _service-container-imports-directive:

``imports`` を使ってコンフィギュレーションをインポートする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

これまで、私たちは ``my_mailer`` のサービスコンテナの定義を(\ ``app/config/config.yml`` といった)
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

Symfony2 で開発するときには、自身で特別に作ったバンドルからコンテナコンフィギュレーションを
インポートするために一般的に ``imports`` ディレクティブを使うことでしょう。
Symfony2 のコアバンドルを含むサードパーティ製バンドルのコンテナコンフィギュレーションは、
通常、コンテナエクステンションというアプリケーションを設定するのにより柔軟かつ
簡単な別の仕組みでロードされます。

エクステンションの動作を簡単に説明しておきましょう。
内部的には、これまで見たきたのと同じように各バンドルにサービスが定義されています。
つまり、バンドル内に1つ以上のコンフィギュレーションリソースファイル（普通はXML）があり、
このファイルにバンドルのパラメータやサービスが定義されています。
バンドルのコンフィギュレーションリソースファイルは、アプリケーションコンフィギュレーションから
``imports`` ディレクティブを使って直接インポートするのではく、バンドル内にある
\ *サービスコンテナエクステンション*\ を起動して処理させます。
サービスコンテナエクステンションはバンドル作成者が次の 2 つのことを行うために作るPHPクラスです。

* バンドルのサービスを設定するために必要な全てのサービスコンテナリソースをインポートする

* バンドルのサービスコンフィギュレーションのフラットなパラメータと相互にやり取りすること無く
  バンドルの設定ができるように、セマンティックで簡単な設定を提供する

言い換えれば、サービスコンテナエクステンションはあなたに代わってバンドルのサービスを設定します。
あなたがすぐに分かるようにと、エクステンションはバンドルを構成するための賢明で高度なインターフェースを提供します。

例として、Symfony2 のコアフレームワークのバンドルである ``FrameworkBundle`` を持ってきました。
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
Symfony2 のコアサービスは読み込まれません。
重要なのは「管理できる」という点です。Symfony2 フレームワークにはいかなる魔法も制御できない機能もありません。

もちろん、 ``FrameworkBundle`` のサービスコンテナエクステンションを単に有効にして
読み込むだけでなく、簡単にバンドルの動作をカスタマイズできます。
サービスが内部でどのように定義されているか気にすることなく、カスタマイズできます。

この場合は ``error_handler``\ 、\ ``csrf_protection``\ 、\ ``router`` などがカスタマイズできます。
内部的には ``FrameworkBundle`` は、自身の特定のサービスを定義し設定するために、ここで指定されたオプションを使います。
バンドルはサービスコンテナに必要な全ての ``parameters`` と ``services`` の作成の面倒をみています。
まだ多くの構成を簡単にカスタマイズすることを可能にしながら。
さらに付け加えていうと、多くのサービスコンテナエクステンションはバリデーション機能を備えるほど優秀です。
オプションを忘れていたり、データ型が間違っている場合には知らせてくれます。

バンドルのインストールや設定時には、どのよにうにサービスをインストールし設定すべきか
バンドルのドキュメントを参照してください。コアのバンドルで利用可能なオプションは
:doc:`リファレンスガイド </reference/index>`\ を参照してください。

.. note::

   元々は、サービスコンテナは ``parameters``, ``services``, ``imports``
   ディレクティブだけを認識します。その他のディレクティブはサービスコンテナ
   エクステンションによって扱われます。

バンドルの設定をユーザーフレンドリーにする方法は、クックブック「:doc:`/cookbook/bundles/extension`\ 」を参照してください。

.. index::
   single: Service Container; Referencing services

サービスの参照（注入）
----------------------

これまでのところ ``my_mailer`` サービスはシンプルでした。 たった 1 つのコンストラクター
引数を受け取る、簡単な設定です。これから学んでいきますが、１つまたはそれ以上の他のサービスに
依存するサービスを作成するときに、コンテナの真の力に気がつくことでしょう。

例として、電子メールメッセージの作成とアドレスのコレクションにメール配信を管理する
のに役立つ、新しいサービス ``NewsletterManager`` があるとします。もちろん
``my_mailer`` はメールを配信において既にとても便利なので、 実際に配信する
メッセージをハンドリングするために ``NewsletterManager`` の内部で使います。
クラスは次のようになります。

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

サービスコンテナを使わなくても、コントローラー内で ``NewsletterManager`` オブジェクトを生成するのは簡単です。

::

    use Acme\HelloBundle\Newsletter\NewsletterManager;

    // ...

    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new NewsletterManager($mailer);
        // ...
    }

このアプローチでも機能しますが、後から ``NewsletterManager`` に第二、第三のコンストラクター
引数を追加する必要がでてきた場合はどうでしょうか。コードをリファクタリングしたり、
クラスをリネームする場合はどうでしょうか？いずれのケースも ``NewsletterManager``
がインスタンス化されている場所を探し変更する必要があります。もちろん、
サービスコンテナにはこういった状況に対応するためのオプションがあります。

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

YAML の場合 ``@my_mailer`` シンタックスを使うことでコンテナは ``my_mailer``
と名付けられたサービスを探し、 ``NewsletterManager`` のコンストラクターに
オブジェクトを渡します。しかしながらこの場合、指定された ``my_mailer`` が存在して
いなければなりません。もし存在していなければ例外が投げられます。
依存を任意のものとして印づけるけこともできます。
この設定については次の節で説明します。

参照を使うことはとても強力な手段であり、依存が明確に定義された独自のサービスクラスを
作ることができます。この例では、\ ``newsletter_manager`` サービスが機能するためには
``my_mailer`` サービスが必要です。サービスコンテナにこの依存を定義する時、
コンテナはオブジェクトのインスタンス作成における全ての仕事を処理します。

任意の依存性: セッターによる注入
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

コンストラクターで依存性を注入するやり方は、依存しているオブジェクトが利用可能な
状態であることを保証するのに優れた方法です。もしクラスに任意の依存を持たせたいの
であれば、セッターによる注入が良い方法かもしれません。
セッターによる注入とは、コンストラクターを通して行うのではなく、メソッド呼び出し
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

    この節で紹介した方法はコンストラクターによる注入(コンストラクターインジェクション)
    セッターによる注入(セッターインジェクション)と呼ばれるものです。Symfony2 のサービス
    コンテナはプロパティによる注入(プロパティインジェクション)もサポートしています。

Request オブジェクトの注入
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.4
    ``request_stack`` サービスは Symfony 2.4 から追加されました。

Symfony2 組み込みのサービスは、どれもほとんど同じように振る舞います。コンテナにより単一のインスタンスが作成され、get時や他のサービスへの注入時には同一のインスタンスが返されます。
標準的な Symfony2 アプリケーションで唯一異なるのが、\ ``request`` サービスです。

``request`` をサービスへ注入しようとすると、
:class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
例外がスローされます。1つのコンテナインスタンスにおいてサブリクエストが生成されることもあり、コンテナのライフタイム中に ``request`` が\ **変更されうる**\ からです。

Symfony 2.4 からは、\ ``request`` サービスを注入する代わりに ``request_stack`` サービスを注入するようにし、サービスの ``getCurrentRequest()`` メソッドを使って Request オブジェクトにアクセスしてください。
以前のバージョンを使っている場合や、この問題について詳しく知りたい場合は、クックブックの記事 :doc:`/cookbook/service_container/scopes` を参照してください。

.. tip::

    コントローラとして使うサービスの場合は、アクションメソッドの引数として ``Request`` オブジェクトが渡されるので、コンテナにより注入する必要はありません。
    詳細は :ref:`book-controller-request-argument` を参照してください。

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

YAML の場合 ``@?`` シンタックスを用いることでコンテナに依存が任意であることを伝えます。
もちろん、任意の依存であることを許容するため ``NewsletterManager`` は書き直さなければ
なりません。

::

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Symfony コアバンドルとサードパーティバンドルのサービス
------------------------------------------------------

Symfony2 や全てのサードパーティ製のバンドルは設定されコンテナ経由でサービスを
取得できるので、それらに簡単にアクセスすることができ自分のサービス内でそれらを
利用することもできます。物事をシンプルに保つために、Symfony2 はデフォルトで
コントローラーをサービスとして定義することを必須としていません。さらに、
Symfony2 Standard Edition のデフォルト構成では、サービスコンテナ全体がコントローラーに注入されます。
例として、ユーザーセッション上に情報を保管する方法をとり上げます。
Symfony2 は ``session`` サービスを提供していて、コントローラーから次のようにアスセスすることができます。

::

    public function indexAction($bar)
    {
        $session = $this->get('session');
        $session->set('foo', $bar);

        // ...
    }

Symfony2 では、Symfony コアやサードパーティバンドルに提供された、テンプレートの
レンダリング (``templating``)、メール送信 (``mailer``)、 リクエスト情報へのアクセス
(``request``)といった各タスクを実行するためのサービスを何度も使うことでしょう。

さらにもうワンステップとして、自身のアプリケーション向けに作ったサービス内で
使うこともできます。
まず始めに、\ ``NewsletterManager`` を修正して、Symfony2の本当の ``mailer`` サービスを(架空の
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
    :ref:`service-container-extension-configuration` で述べられていますが、\ ``swiftmailer`` キーは
    ``SwiftmailerBundle`` からサービスエクステンションを呼び出し、 ``mailer`` サービスを登録します。

.. _book-service-container-tags:

タグ (``tags``)
~~~~~~~~~~~~~~~

Web のブログ記事に「Symfony」や「PHP」とタグ付けするのと同じで、コンテナに設定された
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
``foo.twig.extension`` サービスが、Twig の Twig エクステンションとして登録されるべきだ
ということを知ります。言い換えると、Twig は ``twig.extension`` でタグ付けされた全ての
サービスを探し、それら全てを自動的に Twig エクステンションとして追加します。

それゆえ、タグは自身のサービスを登録したり、またはバンドルによって特別な方法で
使われるよう、Symfony2 やサードパーティ製のバンドルに指示する方法です。


以下は Symfony2 のコアバンドルで利用可能なタグの一覧です。これらのタグは各々
サービスに対して異なる効果があり、多くのタグは追加の引数
(単なる ``name`` パラメーター以上のもの)を必要とします。

Symfony2 のコアフレームワークで利用可能な全てのタグの一覧については
:doc:`/reference/dic_tags` を参照してください。

サービスのデバッグ
------------------

コンソールを使うことで、コンテナにどんなサービスが登録されているか見ることができます。
次のコマンドを実行することで全てのサービスとそのクラスが表示されます。

.. code-block:: bash

    $ php app/console container:debug

デフォルトでは public なサービスが表示されます。次のように --show-private オプション
をつければ private なサービスも表示させることができます。

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

.. _`サービス指向アーキテクチャ`: http://ja.wikipedia.org/wiki/サービス指向アーキテクチャ

.. 2011/07/22 shishi 55da9acdca0c74ab1b80a152c48b3f3d3e5eb62b
.. 2011/08/27 hidenorigoto 
.. 2012/10/13 okada 3f8ca1701dd4723ee107688471edc2f1316f1bd1
.. 2013/10/06 okapon(okada) f4aff75d06c5c341323752640b95101eaa4c1e29 (2.3)
.. 2013/10/19 hidenorigoto adcef5549f87eca2254ad41520ae460cbaa787af
