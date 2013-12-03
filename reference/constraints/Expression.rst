Expression
==========

.. note::

    * 対象バージョン：2.4
    * 翻訳更新日：2013/12/3

.. versionadded:: 2.4
    Expression 制約は Symfony 2.4 から導入されました。

この制約を利用すると、\ :ref:`expression <component-expression-language-examples>` コンポーネントの機能を使ったより複雑で動的なバリデーションを行えます。利用例を知りたい方は、\ `基本的な使い方`_\ を参照してください。
同じような柔軟性を持つ制約である :doc:`/reference/constraints/Callback` も合わせてご参照ください。

+----------------+---------------------------------------------------------------------------------------------------+
| 適用先         | :ref:`class <validation-class-target>` または :ref:`property/method <validation-property-target>` |
+----------------+---------------------------------------------------------------------------------------------------+
| オプション     | - :ref:`expression <reference-constraint-expression-option>`                                      |
|                | - `message`_                                                                                      |
+----------------+---------------------------------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Expression`                                   |
+----------------+---------------------------------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\ExpressionValidator`                          |
+----------------+---------------------------------------------------------------------------------------------------+

基本的な使い方
--------------

``BlogPost`` クラスがあり、\ ``category`` プロパティと ``isTechnicalPost`` プロパティを持つとします。

.. code-block:: php

    namespace Acme\DemoBundle\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    class BlogPost
    {
        private $category;

        private $isTechnicalPost;

        // ...

        public function getCategory()
        {
            return $this->category;
        }

        public function setIsTechnicalPost($isTechnicalPost)
        {
            $this->isTechnicalPost = $isTechnicalPost;
        }

        // ...
    }

このオブジェクトのバリデーションには、次のような特殊な要件があります。

* A) ``isTechnicalPost`` が ``true`` の場合、\ ``category`` は ``php`` または ``symfony`` のいずれかのみ

* B) ``isTechnicalPost`` が ``false`` の場合、\ ``category`` に制約はない

こういった場合に Expression 制約を使って解決することができます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/validation.yml
        Acme\DemoBundle\Model\BlogPost:
            constraints:
                - Expression:
                    expression: "this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()"
                    message: "技術的な投稿の場合、カテゴリは php か symfony のみ"

    .. code-block:: php-annotations

        // src/Acme/DemoBundle/Model/BlogPost.php
        namespace Acme\DemoBundle\Model\BlogPost;
        
        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Expression(
         *     "this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()",
         *     message="技術的な投稿の場合、カテゴリは php か symfony のみ"
         * )
         */
        class BlogPost
        {
            // ...
        }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/validation.xml -->
        <class name="Acme\DemoBundle\Model\BlogPost">
            <constraint name="Expression">
                <option name="expression">
                    this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()
                </option>
                <option name="message">
                    技術的な投稿の場合、カテゴリは php か symfony のみ
                </option>
            </constraint>
        </class>


    .. code-block:: php

        // src/Acme/DemoBundle/Model/BlogPost.php
        namespace Acme\DemoBundle\Model\BlogPost;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class BlogPost
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Expression(array(
                    'expression' => 'this.getCategory() in ["php", "symfony"] or !this.isTechnicalPost()',
                    'message' => '技術的な投稿の場合、カテゴリは php か symfony のみ',
                )));
            }

            // ...
        }

:ref:`expression <reference-constraint-expression-option>` オプションには、バリデーションがパスする場合に ``true`` を返す式を指定します。式言語の構文に関する詳細は、\ :doc:`/components/expression_language/syntax` を参照してください。

式と利用可能な値については、\ :ref:`expression <reference-constraint-expression-option>` オプションの項を参照してください。

利用可能なオプション
--------------------

.. _reference-constraint-expression-option:

expression
~~~~~~~~~~

**タイプ**: ``string`` [:ref:`デフォルトオプション <validation-default-option>`]

評価される式を指定します。\ ``==`` や ``===`` を使った式の評価結果が ``false`` だった場合、バリデーションは失敗します。

式言語の構文に関する詳細は、\ :doc:`/components/expression_language/syntax` を参照してください。

式の中でアクセスできる変数は次の 2 つになっています。

制約に必要な状況に合わせて、式の中で次の変数にアクセスできます。

* ``this``: バリデーションの対象となっているオブジェクト (e.g. BlogPost のインスタンス);
* ``value``: バリデーションの対象となっている値 (プロパティに制約が直接適用されている場合にのみ利用可能)

message
~~~~~~~

**タイプ**: ``string`` **デフォルト値**: ``This value is not valid.``

式の評価結果が ``false`` の場合に、このメッセージが表示されます。

.. 2013/12/3 hidenorigoto 00002508843a5d05a54c0f92deb7ca2d60c90a34
