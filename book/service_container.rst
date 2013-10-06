.. index::
   single: Service Container
   single: Dependency Injection; Container


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

    If you want to know a lot more after reading this chapter, check out
    the :doc:`Dependency Injection Component Documentation</components/dependency_injection/introduction>`.


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

    In this example, the controller extends Symfony's base Controller, which
    gives you access to the service container itself. You can then use the
    ``get`` method to locate and retrieve the ``my_mailer`` service from
    the service container. You can also define your :doc:`controllers as services</cookbook/controller/service>`.
    This is a bit more advanced and not necessary, but it allows you to inject
    only the services you need into your controller.

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

    If you want to use a string that starts with an ``@`` sign as a parameter
    value (i.e. a very safe mailer password) in a yaml file, you need to escape
    it by adding another ``@`` sign (This only applies to the YAML format):

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            # This will be parsed as string "@securepass"
            mailer_password: "@@securepass"

.. note::

    The percent sign inside a parameter or argument, as part of the string, must
    be escaped with another percent sign:

    .. code-block:: xml

        <argument type="string">http://symfony.com/?foo=%%s&bar=%%d</argument>

.. caution::

    You may receive a
    :class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
    when passing the ``request`` service as an argument. To understand this
    problem better and learn how to solve it, refer to the cookbook article
    :doc:`/cookbook/service_container/scopes`.

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

Array Parameters
~~~~~~~~~~~~~~~~

Parameters can also contain array values. See :ref:`component-di-parameters-array`.

別のコンテナコンフィギュレーションリソースをインポートする
----------------------------------------------------------

.. tip::

    この節では、サービスコンフィギュレーション・ファイルを\ *リソース*\ として参照します。
    Symfony2 では、ほとんどのサービスコンフィギュレーションリソースは YAML、XML、PHP といったファイルですが、
    データベースや外部の Web サービスなど、どこからでもコンフィギュレーションを読み込めます。

The service container is built using a single configuration resource
(``app/config/config.yml`` by default). All other service configuration
(including the core Symfony2 and third-party bundle configuration) must
be imported from inside this file in one way or another. This gives you absolute
flexibility over the services in your application.

External service configuration can be imported in two different ways. First,
we'll talk about the method that you'll use most commonly in your application:
the ``imports`` directive. In the following section, we'll introduce the
second method, which is the flexible and preferred method for importing service
configuration from third-party bundles.

.. index::
   single: Service Container; Imports

.. _service-container-imports-directive:

``imports`` を使ってコンフィギュレーションをインポートする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

So far, you've placed your ``my_mailer`` service container definition directly
in the application configuration file (e.g. ``app/config/config.yml``). Of
course, since the ``Mailer`` class itself lives inside the ``AcmeHelloBundle``,
it makes more sense to put the ``my_mailer`` container definition inside the
bundle as well.

First, move the ``my_mailer`` container definition into a new container resource
file inside ``AcmeHelloBundle``. If the ``Resources`` or ``Resources/config``
directories don't exist, create them.

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

The definition itself hasn't changed, only its location. Of course the service
container doesn't know about the new resource file. Fortunately, you can
easily import the resource file using the ``imports`` key in the application
configuration.

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

The ``imports`` directive allows your application to include service container
configuration resources from any other location (most commonly from bundles).
The ``resource`` location, for files, is the absolute path to the resource
file. The special ``@AcmeHello`` syntax resolves the directory path of
the ``AcmeHelloBundle`` bundle. This helps you specify the path to the resource
without worrying later if you move the ``AcmeHelloBundle`` to a different
directory.

.. index::
   single: Service Container; Extension configuration

.. _service-container-extension-configuration:

コンテナエクステンションでコンフィギュレーションをインポートする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When developing in Symfony2, you'll most commonly use the ``imports`` directive
to import container configuration from the bundles you've created specifically
for your application. Third-party bundle container configuration, including
Symfony2 core services, are usually loaded using another method that's more
flexible and easy to configure in your application.

Here's how it works. Internally, each bundle defines its services very much
like you've seen so far. Namely, a bundle uses one or more configuration
resource files (usually XML) to specify the parameters and services for that
bundle. However, instead of importing each of these resources directly from
your application configuration using the ``imports`` directive, you can simply
invoke a *service container extension* inside the bundle that does the work for
you. A service container extension is a PHP class created by the bundle author
to accomplish two things:

* import all service container resources needed to configure the services for
  the bundle;

* provide semantic, straightforward configuration so that the bundle can
  be configured without interacting with the flat parameters of the bundle's
  service container configuration.

In other words, a service container extension configures the services for
a bundle on your behalf. And as you'll see in a moment, the extension provides
a sensible, high-level interface for configuring the bundle.

Take the ``FrameworkBundle`` - the core Symfony2 framework bundle - as an
example. The presence of the following code in your application configuration
invokes the service container extension inside the ``FrameworkBundle``:

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

When the configuration is parsed, the container looks for an extension that
can handle the ``framework`` configuration directive. The extension in question,
which lives in the ``FrameworkBundle``, is invoked and the service configuration
for the ``FrameworkBundle`` is loaded. If you remove the ``framework`` key
from your application configuration file entirely, the core Symfony2 services
won't be loaded. The point is that you're in control: the Symfony2 framework
doesn't contain any magic or perform any actions that you don't have control
over.

Of course you can do much more than simply "activate" the service container
extension of the ``FrameworkBundle``. Each extension allows you to easily
customize the bundle, without worrying about how the internal services are
defined.

In this case, the extension allows you to customize the ``error_handler``,
``csrf_protection``, ``router`` configuration and much more. Internally,
the ``FrameworkBundle`` uses the options specified here to define and configure
the services specific to it. The bundle takes care of creating all the necessary
``parameters`` and ``services`` for the service container, while still allowing
much of the configuration to be easily customized. As an added bonus, most
service container extensions are also smart enough to perform validation -
notifying you of options that are missing or the wrong data type.

When installing or configuring a bundle, see the bundle's documentation for
how the services for the bundle should be installed and configured. The options
available for the core bundles can be found inside the :doc:`Reference Guide</reference/index>`.

.. note::

   Natively, the service container only recognizes the ``parameters``,
   ``services``, and ``imports`` directives. Any other directives
   are handled by a service container extension.

If you want to expose user friendly configuration in your own bundles, read the
":doc:`/cookbook/bundles/extension`" cookbook recipe.

.. index::
   single: Service Container; Referencing services

サービスの参照（注入）
----------------------

So far, the original ``my_mailer`` service is simple: it takes just one argument
in its constructor, which is easily configurable. As you'll see, the real
power of the container is realized when you need to create a service that
depends on one or more other services in the container.

As an example, suppose you have a new service, ``NewsletterManager``,
that helps to manage the preparation and delivery of an email message to
a collection of addresses. Of course the ``my_mailer`` service is already
really good at delivering email messages, so you'll use it inside ``NewsletterManager``
to handle the actual delivery of the messages. This pretend class might look
something like this::

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

Without using the service container, you can create a new ``NewsletterManager``
fairly easily from inside a controller::

    use Acme\HelloBundle\Newsletter\NewsletterManager;

    // ...

    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new NewsletterManager($mailer);
        // ...
    }

This approach is fine, but what if you decide later that the ``NewsletterManager``
class needs a second or third constructor argument? What if you decide to
refactor your code and rename the class? In both cases, you'd need to find every
place where the ``NewsletterManager`` is instantiated and modify it. Of course,
the service container gives you a much more appealing option:

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

In YAML, the special ``@my_mailer`` syntax tells the container to look for
a service named ``my_mailer`` and to pass that object into the constructor
of ``NewsletterManager``. In this case, however, the specified service ``my_mailer``
must exist. If it does not, an exception will be thrown. You can mark your
dependencies as optional - this will be discussed in the next section.

Using references is a very powerful tool that allows you to create independent service
classes with well-defined dependencies. In this example, the ``newsletter_manager``
service needs the ``my_mailer`` service in order to function. When you define
this dependency in the service container, the container takes care of all
the work of instantiating the objects.

任意の依存性: セッターによる注入
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Injecting dependencies into the constructor in this manner is an excellent
way of ensuring that the dependency is available to use. If you have optional
dependencies for a class, then "setter injection" may be a better option. This
means injecting the dependency using a method call rather than through the
constructor. The class would look like this::

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

Injecting the dependency by the setter method just needs a change of syntax:

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

参照を任意にする
----------------

Sometimes, one of your services may have an optional dependency, meaning
that the dependency is not required for your service to work properly. In
the example above, the ``my_mailer`` service *must* exist, otherwise an exception
will be thrown. By modifying the ``newsletter_manager`` service definition,
you can make this reference optional. The container will then inject it if
it exists and do nothing if it doesn't:

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

In YAML, the special ``@?`` syntax tells the service container that the dependency
is optional. Of course, the ``NewsletterManager`` must also be written to
allow for an optional dependency::

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Symfony コアバンドルとサードパーティバンドルのサービス
------------------------------------------------------

Since Symfony2 and all third-party bundles configure and retrieve their services
via the container, you can easily access them or even use them in your own
services. To keep things simple, Symfony2 by default does not require that
controllers be defined as services. Furthermore Symfony2 injects the entire
service container into your controller. For example, to handle the storage of
information on a user's session, Symfony2 provides a ``session`` service,
which you can access inside a standard controller as follows::

    public function indexAction($bar)
    {
        $session = $this->get('session');
        $session->set('foo', $bar);

        // ...
    }

In Symfony2, you'll constantly use services provided by the Symfony core or
other third-party bundles to perform tasks such as rendering templates (``templating``),
sending emails (``mailer``), or accessing information on the request (``request``).

You can take this a step further by using these services inside services that
you've created for your application. Beginning by modifying the ``NewsletterManager``
to use the real Symfony2 ``mailer`` service (instead of the pretend ``my_mailer``).
Also pass the templating engine service to the ``NewsletterManager``
so that it can generate the email content via a template::

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

Configuring the service container is easy:

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

The ``newsletter_manager`` service now has access to the core ``mailer``
and ``templating`` services. This is a common way to create services specific
to your application that leverage the power of different services within
the framework.

.. tip::

    Be sure that the ``swiftmailer`` entry appears in your application
    configuration. As was mentioned in :ref:`service-container-extension-configuration`,
    the ``swiftmailer`` key invokes the service extension from the
    ``SwiftmailerBundle``, which registers the ``mailer`` service.

.. _book-service-container-tags:

タグ (``tags``)
~~~~~~~~~~~~~~~

In the same way that a blog post on the Web might be tagged with things such
as "Symfony" or "PHP", services configured in your container can also be
tagged. In the service container, a tag implies that the service is meant
to be used for a specific purpose. Take the following example:

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

The ``twig.extension`` tag is a special tag that the ``TwigBundle`` uses
during configuration. By giving the service this ``twig.extension`` tag,
the bundle knows that the ``foo.twig.extension`` service should be registered
as a Twig extension with Twig. In other words, Twig finds all services tagged
with ``twig.extension`` and automatically registers them as extensions.

Tags, then, are a way to tell Symfony2 or other third-party bundles that
your service should be registered or used in some special way by the bundle.

The following is a list of tags available with the core Symfony2 bundles.
Each of these has a different effect on your service and many tags require
additional arguments (beyond just the ``name`` parameter).

For a list of all the tags available in the core Symfony Framework, check
out :doc:`/reference/dic_tags`.

Debugging Services
------------------

You can find out what services are registered with the container using the
console. To show all services and the class for each service, run:

.. code-block:: bash

    $ php app/console container:debug

By default only public services are shown, but you can also view private services:

.. code-block:: bash

    $ php app/console container:debug --show-private

You can get more detailed information about a particular service by specifying
its id:

.. code-block:: bash

    $ php app/console container:debug my_mailer

Learn more
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

