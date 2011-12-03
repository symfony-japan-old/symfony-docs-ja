.. 2011/12/03 yanchi dfe0182eac4f39cea0fcacfe20ba5f26a8bd5fc7

Min
===

与えられた数字が最大数*以上*であることを検証します。

+----------------+--------------------------------------------------------------------+
| 適用先         | :ref:`property or method<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| オプション     | - `limit`_                                                         |
|                | - `message`_                                                       |
|                | - `invalidMessage`_                                                |
+----------------+--------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Min`           |
+----------------+--------------------------------------------------------------------+
| バリデータ     | :class:`Symfony\\Component\\Validator\\Constraints\\MinValidator`  |
+----------------+--------------------------------------------------------------------+

基本的な使い方
--------------

クラスの"年齢"フィールドが"18"以上であることを確認するには、次の行を追加します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                age:
                    - Min: { limit: 18, message: You must be 18 or older to enter. }

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Min(limit = "18", message = "You must be 18 or older to enter")
             */
             protected $age;
        }

オプション
----------

limit
~~~~~

**タイプ**: ``integer`` [:ref:`default option<validation-default-option>`]

この必須オプションは、"最小"の値です。与えられた値がこの最小値よりも**小さい**場合、検証は失敗します。

message
~~~~~~~

**タイプ**: ``string`` **デフォルト**: ``This value should be {{ limit }} or more``

基になる値が`limit`_オプションより小さい場合、そのメッセージが表示されます。

invalidMessage
~~~~~~~~~~~~~~

**タイプ**: ``string`` **デフォルト**: ``This value should be a valid number``

基になる値が数値（`IS_NUMERIC`_ PHPの関数によって）でない場合、表示されるメッセージ。

.. _`is_numeric`: http://www.php.net/manual/en/function.is-numeric.php
