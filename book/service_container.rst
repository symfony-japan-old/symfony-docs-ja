.. index::
   single: Service Container
   single: Dependency Injection Container

 サービスコンテナ
==================

モダンな PHP アプリケーションにはたくさんのオブジェクトがあります。あるオブジェクトが E メールメッセージの配信を容易にしている間に、別のオブジェクトは情報をデータベースに永続化できます。あなたのアプリケーションでは、プロダクト一覧を管理するオブジェクトを作成したり、サードパーティの API からのデータを処理するオブジェクトを作成できます。モダンなアプリケーション多くのことを行い、各タスクを扱うために多くのオブジェクトで構成されているのがポイントです。

この章では、インスタンス化を助け、アプリケーションの多くのオブジェクトを組み立て、取り出す Symfony2 における特別な PHP オブジェクトについて解説します。このオブジェクトはサービスコンテナと呼ばれ、アプリケーションを構成するオブジェクトを標準化し、一元化できるようになります。コンテナはエンジニアライフをより簡単に、すごく早くし、そして再利用可能で分離しているコードを促進しするアーキテクチャを重視させます。Symfony2 のコアなクラスはすべてコンテナを使用しているので、Symfony2 におけるあらゆるオブジェクトの拡張や設定、仕様の方法を学ぶことができます。大部分において、サービスコンテナは Symfony2 のスピードや拡張性に最も寄与しています。

最後に、サービスコンテナを設定、使用することは簡単です。本性の終りには、どんなサードパーティのバンドルからでもコンテナやカスタマイズオブジェクトを通して簡単に自分のオブジェクトを作成できるようになるでしょう。サービスコンテナが良いコードを簡単に書かせてくれるので、より再利用可能で、テスト可能で、分離可能なコードを書くようになるでしょう。

.. index::
   single: Service Container; What is a service?

サービスとは何か？
------------------

シンプルに言うと、\ :term:`Service` (サービス) とは「グローバル」と分類されるタスクを実行するあらゆる PHP オブジェクトです。ある目的のため (メール配信など) のため作成されるオブジェクトを説明するため、コンピュータ科学で特定の意図を持って、また一般的に使用される名称です。各サービスはその提供する特定の機能を必要とされるときにアプリケーション内中を通して使用されます。サービスを作成するのに特別なことをする必要はありません。単純に特定のタスクを完遂するクラスとコードを書くだけです。おめでとう、あなたは今サービスを作成しました！

.. note::

   ルールとして、アプリケーションでグローバルに使われるオブジェクトである場合に、PHP オブジェクトはサービスとなります。単独の ``Mailer`` サービスは E メールメッセージを送信するのにグローバルに使用されるのに対し、送信される複数の ``Message`` オブジェクトはサービスでは\ *ありません*\ 。同じように、\ ``Product`` オブジェクトもサービスでありません。しかし、\ ``Product`` オブジェクトをデータベースに永続化するオブジェクトは\ *サービス* です。

さて、何がそんなに良いことなのでしょうか？「サービス」を考えることの利点は、アプリケーション中のサービスの種類ごとに機能の断片に分けて考えれるようになります。各サービスは１つだけの役割を行うので、必要なときに簡単に書くサービスにアクセスし、機能を使うことができます。各サービスはアプリケーション中で他の機能と分かれているので、簡単にテストや、設定ができるようにもなります。この考えは\ `サービス指向アーキテクチャ`_ と呼ばれていて、Symfony2 や PHP 固有のものではありません。独立したサービスクラスの集合でアプリケーションを構成することは、有名で、信用のあるオブジェクト指向のベストプラクティスです。どんな言語でも、これらのスキルは良い開発者になるためのキーです。

.. index::
   single: Service Container; What is?

サービスコンテナとは何か？
-------------------------

:term:`Service Container` (サービスコンテナ) (or *依存性注入コンテナ*) は単純にサービスのインスタンス化を管理する PHP オブジェクト (他の一般的なオブジェクトのように) です。
例えば、E メールメッセージを配信する単純な PHP クラスを考えてみます。サービスコンテナがなければ、必要なときにいつも手動でオブジェクトを作成しなければなりません。

.. code-block:: php

    use Acme\HelloBundle\Mailer;

    $mailer = new Mailer('sendmail');
    $mailer->send('ryan@foobar.net', ... );

とても簡単です。仮想の ``Mailer`` クラスは E メールメッセージを送信する (\ ``sendmail``, ``smtp`` のように) するためのメソッドの設定ができます。
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
        <services>
            <service id="my_mailer" class="Acme\HelloBundle\Mailer">
                <argument>sendmail</argument>
            </service>
        </services>

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

An instance of the ``Acme\HelloBundle\Mailer`` object is now available via
the service container. The container is available in any traditional Symfony2
controller where you can access the services of the container via the ``get()``
shortcut method::

    class HelloController extends Controller
    {
        // ...

        public function sendEmailAction()
        {
            // ...
            $mailer = $this->get('my_mailer');
            $mailer->send('ryan@foobar.net', ... );
        }
    }

When we ask for the ``my_mailer`` service from the container, the container
constructs the object and returns it. This is another major advantage of
using the service container. Namely, a service is *never* constructed until
it's needed. If you define a service and never use it on a request, the service
is never created. This saves memory and increases the speed of your application.
This also means that there's very little or no performance hit for defining
lots of services. Services that are never used are never constructed.

As an added bonus, the ``Mailer`` service is only created once and the same
instance is returned each time you ask for the service. This is almost always
the behavior you'll need (it's more flexible and powerful), but we'll learn
later how you can configure a service that has multiple instances.

.. _book-service-container-parameters:

Service Parameters
------------------

The creation of new services (i.e. objects) via the container is pretty
straightforward. Parameters make defining services more organized and flexible:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        %my_mailer.class%
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
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

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

The end result is exactly the same as before - the difference is only in
*how* we defined the service. By surrounding the ``my_mailer.class`` and
``my_mailer.transport`` strings in percent (``%``) signs, the container knows
to look for parameters with those names. When the container is built, it
looks up the value of each parameter and uses it in the service definition.

The purpose of parameters is to feed information into services. Of course
there was nothing wrong with defining the service without using any parameters.
Parameters, however, have several advantages:

* separation and organization of all service "options" under a single
  ``parameters`` key;

* parameter values can be used in multiple service definitions;

* when creating a service in a bundle (we'll show this shortly), using parameters
  allows the service to be easily customized in your application.

The choice of using or not using parameters is up to you. High-quality
third-party bundles will *always* use parameters as they make the service
stored in the container more configurable. For the services in your application,
however, you may not need the flexibility of parameters.

Importing other Container Configuration Resources
-------------------------------------------------

.. tip::

    In this section, we'll refer to service configuration files as *resources*.
    This is to highlight that fact that, while most configuration resources
    will be files (e.g. YAML, XML, PHP), Symfony2 is so flexible that configuration
    could be loaded from anywhere (e.g. a database or even via an external
    web service).

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
   single: Service Container; imports

.. _service-container-imports-directive:

Importing Configuration with ``imports``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

So far, we've placed our ``my_mailer`` service container definition directly
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
                class:        %my_mailer.class%
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
container doesn't know about the new resource file. Fortunately, we can
easily import the resource file using the ``imports`` key in the application
configuration.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            hello_bundle:
                resource: @AcmeHelloBundle/Resources/config/services.yml

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="@AcmeHelloBundle/Resources/config/services.xml"/>
        </imports>

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

Importing Configuration via Container Extensions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When developing in Symfony2, you'll most commonly use the ``imports`` directive
to import container configuration from the bundles you've created specifically
for your application. Third-party bundle container configuration, including
Symfony2 core services, are usually loaded using another method that's more
flexible and easy to configure in your application.

Here's how it works. Internally, each bundle defines its services very much
like we've seen so far. Namely, a bundle uses one or more configuration
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
a bundle on your behalf. And as we'll see in a moment, the extension provides
a sensible, high-level interface for configuring the bundle.

Take the ``FrameworkBundle`` - the core Symfony2 framework bundle - as an
example. The presence of the following code in your application configuration
invokes the service container extension inside the ``FrameworkBundle``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            secret:          xxxxxxxxxx
            charset:         UTF-8
            form:            true
            csrf_protection: true
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config charset="UTF-8" secret="xxxxxxxxxx">
            <framework:form />
            <framework:csrf-protection />
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <!-- ... -->
        </framework>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'secret'          => 'xxxxxxxxxx',
            'charset'         => 'UTF-8',
            'form'            => array(),
            'csrf-protection' => array(),
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
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

In this case, the extension allows you to customize the ``charset``, ``error_handler``,
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

.. index::
   single: Service Container; Referencing services

Referencing (Injecting) Services
--------------------------------

So far, our original ``my_mailer`` service is simple: it takes just one argument
in its constructor, which is easily configurable. As you'll see, the real
power of the container is realized when you need to create a service that
depends on one or more other services in the container.

Let's start with an example. Suppose we have a new service, ``NewsletterManager``,
that helps to manage the preparation and delivery of an email message to
a collection of addresses. Of course the ``my_mailer`` service is already
really good at delivering email messages, so we'll use it inside ``NewsletterManager``
to handle the actual delivery of the messages. This pretend class might look
something like this::

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

Without using the service container, we can create a new ``NewsletterManager``
fairly easily from inside a controller::

    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new Acme\HelloBundle\Newsletter\NewsletterManager($mailer);
        // ...
    }

This approach is fine, but what if we decide later that the ``NewsletterManager``
class needs a second or third constructor argument? What if we decide to
refactor our code and rename the class? In both cases, you'd need to find every
place where the ``NewsletterManager`` is instantiated and modify it. Of course,
the service container gives us a much more appealing option:

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
                class:     %newsletter_manager.class%
                arguments: [@my_mailer]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer"/>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
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

Optional Dependencies: Setter Injection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
                class:     %newsletter_manager.class%
                calls:
                    - [ setMailer, [ @my_mailer ] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ));

.. note::

    The approaches presented in this section are called "constructor injection"
    and "setter injection". The Symfony2 service container also supports
    "property injection".

Making References Optional
--------------------------

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
                class:     %newsletter_manager.class%
                arguments: [@?my_mailer]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer" on-invalid="ignore" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer', ContainerInterface::IGNORE_ON_INVALID_REFERENCE))
        ));

In YAML, the special ``@?`` syntax tells the service container that the dependency
is optional. Of course, the ``NewsletterManager`` must also be written to
allow for an optional dependency:

.. code-block:: php

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Core Symfony and Third-Party Bundle Services
--------------------------------------------

Since Symfony2 and all third-party bundles configure and retrieve their services
via the container, you can easily access them or even use them in your own
services. To keep things simple, Symfony2 by defaults does not require that
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

We can take this a step further by using these services inside services that
you've created for your application. Let's modify the ``NewsletterManager``
to use the real Symfony2 ``mailer`` service (instead of the pretend ``my_mailer``).
Let's also pass the templating engine service to the ``NewsletterManager``
so that it can generate the email content via a template::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Templating\EngineInterface;

    class NewsletterManager
    {
        protected $mailer;

        protected $templating;

        public function __construct(\Swift_Mailer $mailer, EngineInterface $templating)
        {
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
                class:     %newsletter_manager.class%
                arguments: [@mailer, @templating]

    .. code-block:: xml

        <service id="newsletter_manager" class="%newsletter_manager.class%">
            <argument type="service" id="mailer"/>
            <argument type="service" id="templating"/>
        </service>

    .. code-block:: php

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference('mailer'),
                new Reference('templating')
            )
        ));

The ``newsletter_manager`` service now has access to the core ``mailer``
and ``templating`` services. This is a common way to create services specific
to your application that leverage the power of different services within
the framework.

.. tip::

    Be sure that ``swiftmailer`` entry appears in your application
    configuration. As we mentioned in :ref:`service-container-extension-configuration`,
    the ``swiftmailer`` key invokes the service extension from the
    ``SwiftmailerBundle``, which registers the ``mailer`` service.

.. index::
   single: Service Container; Advanced configuration

Advanced Container Configuration
--------------------------------

As we've seen, defining services inside the container is easy, generally
involving a ``service`` configuration key and a few parameters. However,
the container has several other tools available that help to *tag* services
for special functionality, create more complex services, and perform operations
after the container is built.

Marking Services as public / private
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When defining services, you'll usually want to be able to access these definitions
within your application code. These services are called ``public``. For example,
the ``doctrine`` service registered with the container when using the DoctrineBundle
is a public service as you can access it via::

   $doctrine = $container->get('doctrine');

However, there are use-cases when you don't want a service to be public. This
is common when a service is only defined because it could be used as an
argument for another service.

.. note::

    If you use a private service as an argument to more than one other service,
    this will result in two different instances being used as the instantiation
    of the private service is done inline (e.g. ``new PrivateFooBar()``).

Simply said: A service will be private when you do not want to access it
directly from your code.

Here is an example:

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo
             public: false

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo" public="false" />

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo');
        $definition->setPublic(false);
        $container->setDefinition('foo', $definition);

Now that the service is private, you *cannot* call::

    $container->get('foo');

However, if a service has been marked as private, you can still alias it (see
below) to access this service (via the alias).

.. note::

   Services are by default public.

Aliasing
~~~~~~~~

When using core or third party bundles within your application, you may want
to use shortcuts to access some services. You can do so by aliasing them and,
furthermore, you can even alias non-public services.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo
           bar:
             alias: foo

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo"/>

        <service id="bar" alias="foo" />

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo');
        $container->setDefinition('foo', $definition);

        $containerBuilder->setAlias('bar', 'foo');

This means that when using the container directly, you can access the ``foo``
service by asking for the ``bar`` service like this::

    $container->get('bar'); // Would return the foo service

Requiring files
~~~~~~~~~~~~~~~

There might be use cases when you need to include another file just before
the service itself gets loaded. To do so, you can use the ``file`` directive.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo\Bar
             file: %kernel.root_dir%/src/path/to/file/foo.php

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo\Bar">
            <file>%kernel.root_dir%/src/path/to/file/foo.php</file>
        </service>

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo\Bar');
        $definition->setFile('%kernel.root_dir%/src/path/to/file/foo.php');
        $container->setDefinition('foo', $definition);

Notice that symfony will internally call the PHP function require_once
which means that your file will be included only once per request.

.. _book-service-container-tags:

Tags (``tags``)
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

        <service id="foo.twig.extension" class="Acme\HelloBundle\Extension\FooExtension">
            <tag name="twig.extension" />
        </service>

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

* assetic.filter
* assetic.templating.php
* data_collector
* form.field_factory.guesser
* kernel.cache_warmer
* kernel.event_listener
* monolog.logger
* routing.loader
* security.listener.factory
* security.voter
* templating.helper
* twig.extension
* translation.loader
* validator.constraint_validator

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/service_container/factories`
* :doc:`/cookbook/service_container/parentservices`
* :doc:`/cookbook/controller/service`

.. _`service-oriented architecture`: http://wikipedia.org/wiki/Service-oriented_architecture

.. shishi 55da9acdca0c74ab1b80a152c48b3f3d3e5eb62b

