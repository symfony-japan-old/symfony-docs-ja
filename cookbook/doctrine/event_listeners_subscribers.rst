.. _doctrine-event-config:

イベントリスナーとサブスクライバーを登録する
===========================================

Doctrine は、リッチなイベントシステムをパッケージしており、システム内のほとんどの処理の際にイベントを投げます。つまり、自由に :doc:`services</book/service_container>` を作成することができます。そして、 Doctrine に ``prePersist`` などのアクションがあったときなどに、サービスコンテナに知らせるようにすることができます。例えば、オブジェクトがデータベースに保存されたときに、その処理とは別に検索インデックスを作成するときなどに便利です。

Doctrine は、 Doctine イベントをリッスンできるオブジェクトの２つのタイプを定義しています。それは、リスナーとサブスクライバーです。両方とも似てますが、リスナーの方がわかりやすくなっています。詳細は、 Doctrine のサイトの `The Event System`_ を参照してください。

リスナー/サブスクライバーの設定
-----------------------------------

イベントリスナーやサブスクライバーとして動作するサービスを登録するには、 :ref:`tag<book-service-container-tags>` を参照し、サービスに適切な名前を付ける必要があります。ユースケースに寄りますが、リスナーを全ての DBAL 接続や ORM エンティティマネージャにフックすることができます。また、特定の DBAL 接続やその接続を使用する全てのマネージャにもフックすることができます。
:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection: default
                connections:
                    default:
                        driver: pdo_sqlite
                        memory: true

        services:
            my.listener:
                class: Acme\SearchBundle\Listener\SearchIndexer
                tags:
                    - { name: doctrine.event_listener, event: postPersist }
            my.listener2:
                class: Acme\SearchBundle\Listener\SearchIndexer2
                tags:
                    - { name: doctrine.event_listener, event: postPersist, connection: default }
            my.subscriber:
                class: Acme\SearchBundle\Listener\SearchIndexerSubscriber
                tags:
                    - { name: doctrine.event_subscriber, connection: default }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection driver="pdo_sqlite" memory="true" />
                </doctrine:dbal>
            </doctrine:config>

            <services>
                <service id="my.listener" class="Acme\SearchBundle\Listener\SearchIndexer">
                    <tag name="doctrine.event_listener" event="postPersist" />
                </service>
                <service id="my.listener2" class="Acme\SearchBundle\Listener\SearchIndexer2">
                    <tag name="doctrine.event_listener" event="postPersist" connection="default" />
                </service>
                <service id="my.subscriber" class="Acme\SearchBundle\Listener\SearchIndexerSubscriber">
                    <tag name="doctrine.event_subscriber" connection="default" />
                </service>
            </services>
        </container>

リスナークラスの作成
---------------------------

上記の例では、 ``my.listener`` と呼ばれるサービスが ``postPersist`` イベントで Doctrine リスナーとして設定されています。このサービスのクラスは、必ず ``postPersist`` メソッドを持っており、イベントが投げられたときに呼ばれます。
::

    // src/Acme/SearchBundle/Listener/SearchIndexer.php
    namespace Acme\SearchBundle\Listener;
    
    use Doctrine\ORM\Event\LifecycleEventArgs;
    use Acme\StoreBundle\Entity\Product;
    
    class SearchIndexer
    {
        public function postPersist(LifecycleEventArgs $args)
        {
            $entity = $args->getEntity();
            $entityManager = $args->getEntityManager();
            
            // perhaps you only want to act on some "Product" entity
            if ($entity instanceof Product) {
                // do something with the Product
            }
        }
    }

それぞれのイベントでは、 ``LifecycleEventArgs`` オブジェクトにアクセスができ、イベントのエンティティオブジェクトとエンティティマネージャにアクセスすることができます。

重要なこととして、リスナーは、アプリケーション内の *全て* のエンティティをリッスンすることを忘れないでください。つまり、エンティティの特定の種類の扱いのみを対象としたければ、上にあるようにメソッド内でエンティティのクラス名を調べる必要があります。例えば ``BlogPost`` ではなく、 ``Product`` エンティティを対象にしたいときなどです。

.. _`The Event System`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/events.html

.. 2012/01/04 ganchiku 9818ea3316d4fb8bb7e2a4fb4e7ffe777d05f2af

