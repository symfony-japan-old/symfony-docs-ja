.. translate date 2011/10/30
.. github account taka512
.. original source commit id a8f555b

複数のエンティティマネージャと連携する方法
=========================================

Symfony2のアプリケーションで複数のエンティティマネージャを使用することができます。これらは異なるデータベースや、完全に異なるエンティティセットを利用しようとする場合に必要となります。言い換えれば単一のデータベースに接続する単一のエンティティマネージャは別のデータベースに接続する別のエンティティマネージャが残りを処理している間に、複数のエンティティを処理します。

.. note::

    複数のエンティティマネージャを使用する事はかなり簡単ですが、高度であり通常は必要ではありません。このレイヤーに複雑性を追加する前に、複数のエンティティマネージャの必要性を確認してください。

次の設定コードは、2つのエンティティマネージャを設定する方法を示しています:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            orm:
                default_entity_manager:   default
                entity_managers:
                    default:
                        connection:       default
                        mappings:
                            AcmeDemoBundle: ~
                            AcmeStoreBundle: ~
                    customer:
                        connection:       customer
                        mappings:
                            AcmeCustomerBundle: ~

このケースでは、2つのエンティティマネージャを定義し、``default``と``customer``と名付けている。``default``エンティティマネージャは``AcmeDemoBundle``と``AcmeStoreBundle``のエンティティを管理し、 ``customer``エンティティマネージャは``AcmeCustomerBundle``のエンティティを管理している。

複数のエンティティマネージャを利用する際、あなたはどのエンティティーマネージャを利用するか明確にする必要があります。省略しない場合、エンティティマネージャの名前を指定する必要がありますが、省略するとデフォルトのエンティティマネージャ（すなわち``default``）が返されます。::
    class UserController extends Controller
    {
        public function indexAction()
        {
            // both return the "default" em
            $em = $this->get('doctrine')->getEntityManager();
            $em = $this->get('doctrine')->getEntityManager('default');
            
            $customerEm =  $this->get('doctrine')->getEntityManager('customer');
        }
    }

以前と同じようにDoctrineを利用できます。``default``エンティティマネージャでエンティティを扱う事や ``customer``エンティティマネージャを使用してエンティティを扱えます。

