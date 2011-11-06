サービスを作成するファクトリの使用方法
=======================================

Symfony2 のサービスコンテナは、コンストラクタに特定の引数を渡したり、メソッドを呼び出したり、パラメターのセットをしたりできるオブジェクトの生成を制御する強力な方法を用意しています。しかしながら、あなたのオブジェクトを組み立てるのに必要な全てを用意しているわけではありません。こういった状況のために、オブジェクトの作成をするファクトリを使用することができます。そして、直接オブジェクトを初期化するのではなく、ファクトリのメソッドを呼び出すサービスコンテナを使用することができます。

新しい NewsletterManager オブジェクトを設定し、返すファクトリがあるとしましょう。
::

    namespace Acme\HelloBundle\Newsletter;

    class NewsletterFactory
    {
        public function get()
        {
            $newsletterManager = new NewsletterManager();
            
            // ...
            
            return $newsletterManager;
        }
    }

``NewsletterFactory`` ファクトリクラスを使用してサービスコンテナを設定することで、 ``NewsletterManager`` オブジェクトをサービスとして使用可能にすることができます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_manager:
                class:          %newsletter_manager.class%
                factory_class:  %newsletter_factory.class%
                factory_method: get 

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_manager" 
                     class="%newsletter_manager.class%"
                     factory-class="%newsletter_factory.class%"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->setFactoryClass(
            '%newsletter_factory.class%'
        )->setFactoryMethod(
            'get'
        );

``factory_class`` を介してファクトリに使用するクラスを指定した際は、メソッドはスタティックに呼ばれます。この例のように、ファクトリ自身がインスタンス化される必要があり、そのインスタンス化されたオブジェクトのメソッドを呼び出すには、ファクトリ自体をサービスとして設定してください。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_factory:
                class:            %newsletter_factory.class%
            newsletter_manager:
                class:            %newsletter_manager.class%
                factory_service:  newsletter_factory
                factory_method:   get 

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_factory" class="%newsletter_factory.class%"/>
            <service id="newsletter_manager" 
                     class="%newsletter_manager.class%"
                     factory-service="newsletter_factory"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->setFactoryService(
            'newsletter_factory'
        )->setFactoryMethod(
            'get'
        );

.. note::

   ファクトリサービスは、 id の名前で指定されますが、サービス自身への関係は持っていません。そのため、 @ syntax を使用する必要はありません。

ファクトリメソッドへ引数を渡す
---------------------------------------

ファクトリメソッドに引数を渡す必要があれば、サービスコンテナ内で ``arguments`` オプションを呼ぶことができます。例えば、前の例の ``get`` メソッドが ``templating`` サービスを引数として受けるとしますと、次のようになります。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_factory:
                class:            %newsletter_factory.class%
            newsletter_manager:
                class:            %newsletter_manager.class%
                factory_service:  newsletter_factory
                factory_method:   get
                arguments:
                    -             @templating

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_factory" class="%newsletter_factory.class%"/>
            <service id="newsletter_manager" 
                     class="%newsletter_manager.class%"
                     factory-service="newsletter_factory"
                     factory-method="get"
            >
                <argument type="service" id="templating" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('templating'))
        ))->setFactoryService(
            'newsletter_factory'
        )->setFactoryMethod(
            'get'
        );

.. 2011/11/06 ganchiku bffbf219749aeac4113ad237d711e2c0fd9aab8c

