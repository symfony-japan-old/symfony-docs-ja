Choice
======

.. Validates that a value is one or more of a list of choices.

値が1つあるいは複数の選択リストであるかのバリデーションを実行します。

.. code-block:: yaml

    properties:
        gender:
            - Choice: [male, female]

オプション
----------

.. * ``choices`` (**default**, required): The available choices
   * ``callback``: Can be used instead of ``choices``. A static callback method
     returning the choices. If you pass a string, it is expected to be
     the name of a static method in the validated class.
   * ``multiple``: Whether multiple choices are allowed. Default: ``false``
   * ``min``: The minimum amount of selected choices
   * ``max``: The maximum amount of selected choices
   * ``message``: The error message if validation fails
   * ``minMessage``: The error message if ``min`` validation fails
   * ``maxMessage``: The error message if ``max`` validation fails

* ``choices`` (**デフォルト**, 必須): 使用可能な選択肢。
* ``callback``: ``choices`` の代わりに使用可能。選択肢を返すスタティックなコールバックメソッド。文字列を渡すと、バリデーション対象のクラス内のスタティックなメソッド名と想定する。
* ``multiple``: 複数の選択肢が許可されているかどうか。デフォルト: ``false``
* ``min``: 選択できる選択肢の最大個数。
* ``max``: 選択できる選択肢の最大個数。
* ``message``: バリデーションが失敗した時のエラーメッセージ。
* ``minMessage``: ``min`` のバリデーションが失敗した時のエラーメッセージ
* ``maxMessage``: ``max`` のバリデーションが失敗した時のエラーメッセージ


例 1: 静的配列としての選択肢
----------------------------

.. If the choices are few and easy to determine, they can be passed to the
   constraint definition as array.

選択肢の数がほとんどなく、簡単に見つけられる場合、配列として制約の定義を渡すことができます。

.. configuration-block::

    .. code-block:: yaml

        # Sensio/HelloBundle/Resources/config/validation.yml
        Sensio\HelloBundle\Author:
            properties:
                gender:
                    - Choice: [male, female]

    .. code-block:: xml

        <!-- Sensio/HelloBundle/Resources/config/validation.xml -->
        <class name="Sensio\HelloBundle\Author">
            <property name="gender">
                <constraint name="Choice">
                    <value>male</value>
                    <value>female</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Sensio/HelloBundle/Author.php
        class Author
        {
            /**
             * @validation:Choice({"male", "female"})
             */
            protected $gender;
        }

    .. code-block:: php

        // Sensio/HelloBundle/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array('male', 'female')));
            }
        }

例 2: コールバックからの選択肢
------------------------------

.. When you also need the choices in other contexts (such as a drop-down box in
   a form), it is more flexible to bind them to your domain model using a static
   callback method.

他のコンテキスト (例えばフォーム内のドロップダウンなど) で選択肢が必要な場合も、それらをスタティックなコールバックメソッドを使用したドメインモデルに柔軟に関連付けることができます。

.. code-block:: php

    // Sensio/HelloBundle/Author.php
    class Author
    {
        public static function getGenders()
        {
            return array('male', 'female');
        }
    }

.. You can pass the name of this method to the ``callback`` option of the ``Choice``
   constraint.

メソッド名を ``Choice`` 制約のコールバックオプションに渡すことができます。

.. configuration-block::

    .. code-block:: yaml

        # Sensio/HelloBundle/Resources/config/validation.yml
        Sensio\HelloBundle\Author:
            properties:
                gender:
                    - Choice: { callback: getGenders }

    .. code-block:: xml

        <!-- Sensio/HelloBundle/Resources/config/validation.xml -->
        <class name="Sensio\HelloBundle\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="callback">getGenders</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Sensio/HelloBundle/Author.php
        class Author
        {
            /**
             * @validation:Choice(callback = "getGenders")
             */
            protected $gender;
        }

.. If the static callback is stored in a different class, for example ``Util``,
   you can pass the class name and the method as array.

``Util`` のようにスタティックなコールバックが別なクラスに保存される場合、クラス名とメソッドを配列として渡すことができます。

.. configuration-block::

    .. code-block:: yaml

        # Sensio/HelloBundle/Resources/config/validation.yml
        Sensio\HelloBundle\Author:
            properties:
                gender:
                    - Choice: { callback: [Util, getGenders] }

    .. code-block:: xml

        <!-- Sensio/HelloBundle/Resources/config/validation.xml -->
        <class name="Sensio\HelloBundle\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="callback">
                        <value>Util</value>
                        <value>getGenders</value>
                    </option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Sensio/HelloBundle/Author.php
        class Author
        {
            /**
             * @validation:Choice(callback = {"Util", "getGenders"})
             */
            protected $gender;
        }
