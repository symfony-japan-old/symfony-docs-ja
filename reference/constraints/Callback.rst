Callback
========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

Callback アサーションの目的は、独自のバリデーションルールを作り、オブジェクトの特定のフィールドに対して任意のバリデーションエラーを割り当てることです。
フォームでバリデーションを使う場合、独自のバリデーションエラーを単純にフォームの先頭に表示するのではなく、エラーの対象となっているフィールドの隣に表示できます。

この処理は、1 つ以上の *コールバック* メソッドを指定することで、バリデーション処理中に呼び出されるようになります。それぞれのコールバックメソッド内では、バリデーションエラーを生成してフィールドに割り当てる処理や、その他の任意の処理を実行できます。

.. note::

    コールバックメソッド自体が *失敗したり* 何か値を返してはいけません。
    コールバックメソッド内では、例で示すように直接バリデータの "バイオレーション" を追加します。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`クラス<validation-class-target>`                                 |
+----------------+------------------------------------------------------------------------+
| オプション     | - `methods`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Callback`          |
+----------------+------------------------------------------------------------------------+
| バリデータ     | :class:`Symfony\\Component\\Validator\\Constraints\\CallbackValidator` |
+----------------+------------------------------------------------------------------------+

セットアップ
------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            constraints:
                - Callback:
                    methods:   [isAuthorValid]

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Callback(methods={"isAuthorValid"})
         */
        class Author
        {
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <constraint name="Callback">
                <option name="methods">
                    <value>isAuthorValid</value>
                </option>
            </constraint>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Callback(array(
                    'methods' => array('isAuthorValid'),
                )));
            }
        }

コールバックメソッド
--------------------

コールバックメソッドには、引数で ``ExecutionContextInterface`` が渡されます。このオブジェクトに対して直接 "バイオレーション" を追加し、エラーがどのフィールドで発生したのかを特定できるようにします。

.. code-block:: php

    // ...
    use Symfony\Component\Validator\ExecutionContextInterface;
    
    class Author
    {
        // ...
        private $firstName;
    
        public function isAuthorValid(ExecutionContextInterface $context)
        {
            // somehow you have an array of "fake names"
            $fakeNames = array();
        
            // check if the name is actually a fake name
            if (in_array($this->getFirstName(), $fakeNames)) {
                $context->addViolationAt('firstname', 'This name sounds totally fake!', array(), null);
            }
        }
    }

オプション
----------

methods
~~~~~~~

**タイプ**: ``array`` **デフォルト**: ``array()`` [:ref:`デフォルトオプション<validation-default-option>`]

バリデーション処理中に呼び出されるメソッド配列を指定します。各メソッドは、次のフォーマットのいずれかで指定します。

1) **文字列のメソッド名**

    メソッドの名前が ``isAuthorValid`` のような単純な文字列の場合は、バリデーション対象のオブジェクトと同一オブジェクトで、メソッドが呼び出されます。この場合 ``ExecutionContextInterface`` のみがメソッドの引数として渡されます。

2) **静的な配列によるコールバック**

    各メソッドは、PHP 標準の配列形式のコールバックで指定することもできます。

    .. configuration-block::

        .. code-block:: yaml

            # src/Acme/BlogBundle/Resources/config/validation.yml
            Acme\BlogBundle\Entity\Author:
                constraints:
                    - Callback:
                        methods:
                            -    [Acme\BlogBundle\MyStaticValidatorClass, isAuthorValid]

        .. code-block:: php-annotations

            // src/Acme/BlogBundle/Entity/Author.php
            use Symfony\Component\Validator\Constraints as Assert;

            /**
             * @Assert\Callback(methods={
             *     { "Acme\BlogBundle\MyStaticValidatorClass", "isAuthorValid"}
             * })
             */
            class Author
            {
            }

        .. code-block:: xml

            <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
            <class name="Acme\BlogBundle\Entity\Author">
                <constraint name="Callback">
                    <option name="methods">
                        <value>Acme\BlogBundle\MyStaticValidatorClass</value>
                        <value>isAuthorValid</value>
                    </option>
                </constraint>
            </class>

        .. code-block:: php

            // src/Acme/BlogBundle/Entity/Author.php

            use Symfony\Component\Validator\Mapping\ClassMetadata;
            use Symfony\Component\Validator\Constraints\Callback;

            class Author
            {
                public $name;

                public static function loadValidatorMetadata(ClassMetadata $metadata)
                {
                    $metadata->addConstraint(new Callback(array(
                        'methods' => array('isAuthorValid'),
                    )));
                }
            }

    この場合、\ ``Acme\BlogBundle\MyStaticValidatorClass`` クラスのスタティックメソッド ``isAuthorValid`` が呼び出されます。
    メソッドには、バリデート対象のオブジェクト（たとえば ``Author``\ ）と ``ExecutionContextInterface``\ が渡されます。

    .. code-block:: php

        namespace Acme\BlogBundle;
    
        use Symfony\Component\Validator\ExecutionContextInterface;
        use Acme\BlogBundle\Entity\Author;
    
        class MyStaticValidatorClass
        {
            public static function isAuthorValid(Author $author, ExecutionContextInterface $context)
            {
                // ...
            }
        }

    .. tip::

        ``Callback`` 制約を PHP コードから指定する場合は、PHP のクロージャや、スタティックではないコールバックを使うこともできます。
        しかし、現時点では\ :term:`サービス`\ を制約として使うことはサポートされていません。
        サービスを使ってバリデーションを行いたい場合は、\ :doc:`カスタムバリデーション制約の作成方法</cookbook/validation/custom_constraint>` を参照し、クラスに新しい制約を追加してください。

.. 2011/01/27 hidenorigoto 34ab8860ec2510047e9c1329e63529506bec5004
.. 2013/06/07 hidenorigoto e6332ab6b0d6b1d4bc30ffa91c75935e8aa6c4be
