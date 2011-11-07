親サービスと共通の依存(dependency)をマネージする方法
======================================================

アプリケーションにさらに機能を追加していくと、同じ依存(dependency)を共有する関連したクラスを持つことになるでしょう。例えば、セッターの注入(injection) を使用して依存(dependency)をセットする Newsletter Manager があるとしましょう。
::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    class NewsletterManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }
        // ...
    }

そして、グリーティングカードクラスも同じ依存(dependency)を共有しています。
::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    class GreetingCardManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }
        // ...
    }

これらのクラスのサービスコンフィギュレーションは、以下のようになります。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HelloBundle\Mail\GreetingCardManager
        services:
            my_mailer:
                # ...
            my_email_formatter:
                # ...
            newsletter_manager:
                class:     %newsletter_manager.class%
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]

            greeting_card_manager:
                class:     %greeting_card_manager.class%
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HelloBundle\Mail\GreetingCardManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'Acme\HelloBundle\Mail\GreetingCardManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('greeting_card_manager', new Definition(
            '%greeting_card_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));

両方のクラスとコンフィギュレーションに重複がたくさんあります。つまり、 ``EmailFormatter`` クラスの ``Mailer`` をコンテナから注入(inject)するなど、何か変更があった際に、二箇所でコンフィギュレーションの変更が必要になります。同様に、セッターメソッドを変更するには、両方のクラスの変更も必要になります。これらの関連したクラスの共通したメソッドを対処するには、重複したコードをスーパークラスに移動させることです。
::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    abstract class MailManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }
        // ...
    }

そして、 ``NewsletterManager`` と ``GreetingCardManager`` は、このスーパークラスを拡張します。
::

    namespace Acme\HelloBundle\Mail;

    class NewsletterManager extends MailManager
    {
        // ...
    }

::

    namespace Acme\HelloBundle\Mail;

    class GreetingCardManager extends MailManager
    {
        // ...
    }

同様に、 Symfony2 のサービスコンテナは、コンフィギュレーションでサービスも拡張することができます。そうすることによって、親サービスを特定し、重複を減らすことができます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HelloBundle\Mail\GreetingCardManager
            mail_manager.class: Acme\HelloBundle\Mail\MailManager
        services:
            my_mailer:
                # ...
            my_email_formatter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]
            
            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
            
            greeting_card_manager:
                class:     %greeting_card_manager.class%
                parent: mail_manager
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HelloBundle\Mail\GreetingCardManager</parameter>
            <parameter key="mail_manager.class">Acme\HelloBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager"/>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'Acme\HelloBundle\Mail\GreetingCardManager');
        $container->setParameter('mail_manager.class', 'Acme\HelloBundle\Mail\MailManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        );
        $container->setDefinition('greeting_card_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

この文脈では、 ``parent`` サービスを持っているということは、親サービスの引数とメソッドの呼び出しが個サービスとして使われることを意味します。そして、親サービスで定義されたセッターメソッドは、子サービスが初期化されたときに、呼ばれます。

.. note::

   コンフィギュレーションで、キーの ``parent`` を指定しなくても、 ``MailManager`` クラスを拡張したサービスが初期化されます。 ``parent`` のキーを指定しないことによって得られる違いは、子サービスが初期化されたときに ``mail_manager`` サービスで定義された  ``calls`` が実行されないことです。

親クラスは抽象クラスなので、直接初期化されることはありません。上記のようにコンフィギュレーションファイルで ``abstract`` と指定しましたので、このクラスは、親サービスとしてのみ使用され、注入(inject)するサービスとして直接使われることがなく、コンパイルの際に取り除かれる、ということを意味します。つまり、単に他のサービスが使う "template" として存在しているのです。

親の依存(dependency)をオーバーライドする
------------------------------

子サービスのみの依存(dependency)専用に渡すクラスをオーバーライドしたいときもあるでしょう。幸いなことに、子サービスのコンフィギュレーションのメソッド呼び出し(calls)を追加することで、親クラスでセットされた依存(dependency)をオーバーライドすることができます。例えば、 ``NewsletterManager`` クラスのみに異なる依存(dependency)を渡したいときは、コンフィギュレーションは以下のようになります。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HelloBundle\Mail\GreetingCardManager
            mail_manager.class: Acme\HelloBundle\Mail\MailManager
        services:
            my_mailer:
                # ...
            my_alternative_mailer:
                # ...
            my_email_formatter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]
            
            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
                calls:
                    - [ setMailer, [ @my_alternative_mailer ] ]
            
            greeting_card_manager:
                class:     %greeting_card_manager.class%
                parent: mail_manager
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HelloBundle\Mail\GreetingCardManager</parameter>
            <parameter key="mail_manager.class">Acme\HelloBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_alternative_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setMailer">
                     <argument type="service" id="my_alternative_mailer" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'Acme\HelloBundle\Mail\GreetingCardManager');
        $container->setParameter('mail_manager.class', 'Acme\HelloBundle\Mail\MailManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_alternative_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setMailer', array(
            new Reference('my_alternative_mailer')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

これで ``GreetingCardManager`` は以前と同じ依存(dependency)を受け取りますが、 ``NewsletterManager`` は ``my_mailer`` サービスの代わりに ``my_alternative_mailer`` を受け取るようになりました。

依存(dependency)の集合
---------------------------

上記の例では、オーバーライドしたセッターメソッドは、実際、二度呼ばれます。一度目は、親の定義として、二度目は、子の定義としてです。この例では、二度目の ``setMailer`` 呼び出しが一度目の呼び出しによってセットされたオブジェクトを置き換えることになるので、大丈夫でした。

しかし、この二度呼び出しが問題になるときもあります。例えば、オーバーライドしたメソッド呼び出しがコレクションに追加するときなどです。その際には、そのコレクションに両方のオブジェクトが追加されてしまいます。以下ではこのケースであり、親クラスは次のようになります。
::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    abstract class MailManager
    {
        protected $filters;

        public function setFilter($filter)
        {
            $this->filters[] = $filter;
        }
        // ...
    }

以下のようなコンフィギュレーションをしているとします

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            mail_manager.class: Acme\HelloBundle\Mail\MailManager
        services:
            my_filter:
                # ...
            another_filter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setFilter, [ @my_filter ] ]
                    
            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
                calls:
                    - [ setFilter, [ @another_filter ] ]
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="mail_manager.class">Acme\HelloBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="my_filter" ... >
              <!-- ... -->
            </service>
            <service id="another_filter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setFilter">
                     <argument type="service" id="my_filter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setFilter">
                     <argument type="service" id="another_filter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('mail_manager.class', 'Acme\HelloBundle\Mail\MailManager');

        $container->setDefinition('my_filter', ... );
        $container->setDefinition('another_filter', ... );
        $container->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setFilter', array(
            new Reference('my_filter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setFilter', array(
            new Reference('another_filter')
        ));

この例では、 ``newsletter_manager`` サービスの ``setFilter`` メソッドが二度呼ばれ、 ``$filters`` 配列には、 ``my_filter`` オブジェクトと ``another_filter`` オブジェクトの両方が入ることになります。こうしてサブクラスに追加フィルターを追加できるのは、良いことです。また、サブクラスに渡されたフィルターを置き換えるには、コンフィギュレーションから親の設定を取り除いて、ベースクラスが ``setFilter`` メソッドを呼ばないようにしてください。

.. 2011/11/07 ganchiku c2eb6898fff01bb9b500cb2e70bb8ebb5ea9c2d6

