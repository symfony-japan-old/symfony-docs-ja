False
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

値が ``false`` であることを検証します。厳密には、値が ``false``\ 、整数の ``0``\ 、文字列の "``0``" であることを検証します。
:doc:`True <True>` も参照してください。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\False`          |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\FalseValidator` |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

``False`` 制約はプロパティまたは "getter" メソッドに適用できます。"getter" に対して適用するのがよくあるケースです。たとえば、\ ``state`` プロパティの値が ``invalidStates`` 配列に存在\ *しない*\ ことを保証したいとします。最初に次のように "getter" メソッドを作ります。

.. code-block:: php

    protected $state;

    protected $invalidStates = array();

    public function isStateInvalid()
    {
        return in_array($this->state, $this->invalidStates);
    }

ここでは、対象オブジェクトの ``isStateInvalid`` メソッドが **false** を返した時にのみ有効であるとします。

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            getters:
                stateInvalid:
                    - "False":
                        message: You've entered an invalid state.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\False(
             *     message = "You've entered an invalid state."
             * )
             */
             public function isStateInvalid()
             {
                // ...
             }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="stateInvalid">
                <constraint name="False">
                    <option name="message">You've entered an invalid state.</option>
                </constraint>
            </getter>
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
                $metadata->addGetterConstraint('stateInvalid', new Assert\False());
            }
        }

.. caution::

    YAML を使う場合、\ ``False``\ を二重引用符で囲う (``"False"``) ことに注意してください。
    これを忘れると、単なるブール値として扱われてしまいます。

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be false``

検証対象のデータが false ではない場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 5f4e6ef7fa26bd9aa160a3f01e3e1f25505de1c0
