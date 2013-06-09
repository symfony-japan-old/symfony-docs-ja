Time
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

検証対象の値が有効な時刻であることを検証します。有効な時刻とは、\ ``DateTime`` オブジェクトか、"HH:MM:SS" 形式の文字列 (文字列にキャスト可能なオブジェクトも含む) を指します。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`            |
+----------------+------------------------------------------------------------------------+
| オプション     | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Time`              |
+----------------+------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\TimeValidator`     |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

Event クラスに ``startAt`` フィールドがあり、イベント開始時刻を保持しているとします。このフィールドの値が時刻であることを保証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Event:
            properties:
                startsAt:
                    - Time: ~

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Event.php
        namespace Acme\EventBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Event
        {
            /**
             * @Assert\Time()
             */
             protected $startsAt;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Event">
            <property name="startsAt">
                <constraint name="Time" />
            </property>
        </class>

    .. code-block:: php
        
        // src/Acme/EventBundle/Entity/Event.php
        namespace Acme\EventBundle\Entity;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Event
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('startsAt', new Assert\Time());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid time``

検証対象のデータが有効な時刻ではない場合、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 72a6d59266859764764ef148d4acd44087b86dfa
